/*
  ==============================================================================

    CircularBuffer.h
    Created: 23 Jun 2020 8:13:21am
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

/*
 Properties:
 - owns 1-2 channels AudioBuffer, start with just 1
 - writes and reads sample wise (compare performance)
 - can process frames
 - reads w/ or w/o interpolation
 - various numbers of taps
 
 How to:
- write it, so the DDL can be used for Comb, Allpass etc.?
 - how do increment write position? in the CircularBuffer? or as before in processBlock?
 
 */

/*
 CircularBuffer
 - owns AudioBuffer of N channels and size M
 - owns a writing position to that Buffer - writePosition
 - single read position (fixed or fractional) ? Or delegate it to Delay object?
 
 Delay Object for multiple taps
 - keeps member of CircularBuffer
 - N taps - N readIndecies
 - TAP controls - On/Off, Gain, Pan, DelayInMs
 - convert Ms to Samples, CircularBuffer must only operate on samples
 - Only one tap goes to feedback, which one?
 
 */


namespace rosen
{
    
enum class Interpolation
{
    none, linear, cosine, cubic, lagrange, spline
    
};

    
//==============================================================================
// CIRCULARBUFFER CLASS
//==============================================================================
    
// this is a simple DDL. Only 1 channel buffer here
// T is a float or double
template <typename T>
class CircularBuffer
{
public:
    
    CircularBuffer();
    CircularBuffer(int sizeInMs, double sampleRate);
    ~CircularBuffer()
    {}
    
    unsigned int getBufferSize() const {return bufferSize;}
    void clearBuffer() {circularBuffer.clear();}
    void numBufferChannels() const {circularBuffer.numChannels();}
    
    void writeBlock(AudioBuffer<T>& buffer, const int& channelToCopy, T gainStart, T gainEnd, bool replace = true) noexcept;
    void advanceWritePosition(const int& numSamples) noexcept;

    T readSample(T delayFraction, int delayBufferChannel = 0, Interpolation interpolate = Interpolation::none) noexcept;
    T readSample(int delayInSamples) noexcept;
    
private:

    unsigned int bufferSize;
    unsigned int writePosition;
    unsigned int wrapMask;
    int channels;
    AudioBuffer<T> circularBuffer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CircularBuffer)
};
    
template <typename T>
CircularBuffer<T>::CircularBuffer()
:bufferSize{1 << 18}, writePosition{0}, channels{1}
    {
        wrapMask = bufferSize - 1;
        circularBuffer.setSize(channels, bufferSize);
        circularBuffer.clear(); // clear the buffer and set all values to 0
    }

template <typename T>
CircularBuffer<T>::CircularBuffer(int sizeInMs, double sampleRate)
:writePosition{0}, channels{1}
    {
        double requestedSizeInSamples = ((double)sizeInMs / 1000.0) * sampleRate;
        // adjusting the size to binary
        bufferSize = (unsigned)(pow(2, ceil(log2(requestedSizeInSamples))));
        circularBuffer.setSize(channels, bufferSize);
        circularBuffer.clear();
    }
    
    
// sample-wise write leave for CircularBuffer, but delegate entire block to Delay?
template <typename T>
void CircularBuffer<T>::writeBlock(AudioBuffer<T>& buffer, const int& channelToCopy, T gainStart, T gainEnd, bool replace) noexcept
{
    // get a pointer to the buffer channel
    auto* bufferData = buffer.getReadPointer(channelToCopy);
    auto localWritePos = writePosition;
    
    T gainIncrement = (gainEnd - gainStart) / buffer.getNumSamples();
    auto localGain = gainStart;
    
//    for (int i = 0; i < buffer.getNumSamples(); ++i)
//        {
//            auto sample = bufferData[i] * (gainStart + gainIncrement);
//            if (replace)
//            {
//                // if direct signal - override the sample
//                circularBuffer->get()[localWritePos] = sample;
//            }
//            // in case of feedback, add to the sample
//            else
//            {
//                circularBuffer->get()[localWritePos] += sample;
//            }
//
//            ++localWritePos;
//            localWritePos &= wrapMask;
//        }
    
    // direct signal - the sample gets replaced
    if (replace)
    {
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            
            circularBuffer[localWritePos] = bufferData[i] * localGain;;
            localGain += gainIncrement;
            ++localWritePos;
            localWritePos &= wrapMask;
        }
    }
    // feedback - the sample is summed up with the feedback sample
    else
    {
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            
            circularBuffer[localWritePos] += (bufferData[i] * localGain);
            localGain += gainIncrement;
            ++localWritePos;
            localWritePos &= wrapMask;
        }
    }
}

template <typename T>
void CircularBuffer<T>::advanceWritePosition(const int& numSamples) noexcept
{
    writePosition += numSamples;
    if (writePosition > bufferSize - 1)
        writePosition -= bufferSize;
}
    
    // read sample w/o interpolation
    template <typename T>
    T CircularBuffer<T>::readSample(int delayInSamples) noexcept
    {
        auto* circularData = circularBuffer.getReadPointer(0);
        int readPosition = writePosition - delayInSamples;
        //readPosition &= wrapMask;
        if (readPosition > bufferSize - 1) readPosition -= bufferSize;
        return circularData[readPosition];
    }

// read sample with interpolation
template <typename T>
T CircularBuffer<T>::readSample(T delayFraction, int delayBufferChannel, Interpolation interpolate) noexcept
{
    // delayFraction is a read position with fraction
    // what if delayFraction is out of bound, so delayFraction > bufferSize?y
    auto* circularData = circularBuffer.getReadPointer(0);
    // trancate to floor index
    T readPosition = (T)writePosition - delayFraction;
    if (readPosition > (T)bufferSize - 1) readPosition -= (T)bufferSize;
    T loValue = circularData[(int)readPosition];
    
    if (interpolate == Interpolation::linear)
    {
        int upperIndex = (int)readPosition + 1;
        upperIndex &= wrapMask;
        T hiValue = circularData[upperIndex];
        return linearInterpolation<T>(readPosition, loValue, hiValue);
    }
    
    return loValue; // if Interpolation::none
}

// interpolation algorithms as helper functions?

template <typename T>
T linearInterpolation(T fractionalValue, const T& loValue, const T& hiValue)
{
    T fraction = fractionalValue - (int)fractionalValue;
    return loValue + fraction * (hiValue - loValue);;
}
    
//==============================================================================
// DELAY CLASS
//==============================================================================

template <typename T>
class Delay
{
public:
    Delay()
    :currentSampleRate{0}, delayInSamples{0}
    {
    }
    
    Delay(int bufferLength, double sampleRate)
    :currentSampleRate{sampleRate}
    {
        delayBuffer = std::make_unique<CircularBuffer<T>>(bufferLength, sampleRate);
    }
    
    void setDelayInMs(const T& delayInMs) noexcept;
    forcedinline void advanceWrite(const int& numSamples) {delayBuffer.get()->advanceWritePosition(numSamples);}
    forcedinline void write(AudioBuffer<T>& buffer, const int& bufChannel, T gainStart, T gainEnd, bool replace) noexcept;
    void read(AudioBuffer<T>& buffer, const int& bufChannel, T gainStart, T gainEnd, bool withInterpolation = false, Interpolation interpolationType = Interpolation::none) noexcept;
private:
    std::unique_ptr<CircularBuffer<T>> delayBuffer;
    double currentSampleRate;
    double delayInSamples; // if one tap only, for multiple taps, create a dynamic array (std::vector)
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Delay)
        
};
    
    
    template <typename T>
    void Delay<T>::setDelayInMs(const T& delayInMs) noexcept
    {
        delayInSamples = delayInMs * currentSampleRate / 1000.0;
    }
    
    template <typename T>
    forcedinline void Delay<T>::write(AudioBuffer<T> &buffer, const int &bufChannel, T gainStart, T gainEnd, bool replace) noexcept
    {
        if (currentSampleRate != 0 && delayBuffer.get()->getBufferSize() > 0)
        {
            delayBuffer.get()->writeBlock(buffer, bufChannel, gainStart, gainEnd, replace);
        }
    }
    
    template <typename T>
    void Delay<T>::read(AudioBuffer<T>& buffer, const int& bufChannel, T gainStart, T gainEnd, bool withInterpolation, Interpolation interpolationType) noexcept
    {
        auto* bufferData = buffer.getWritePointer(bufChannel);
        auto gainIncrementPerSample = (gainEnd - gainStart) / buffer.getNumSamples();
        auto localGain = gainStart;
        // no interpolation
        if (!withInterpolation)
        {
            // sample-wise, or pass buffer to delayBuffer object?
            for (int i = 0; i < buffer.getNumSamples(); ++i)
            {
                // how to move the read position?
                // TEST THIS!
                // delayInSamples + 1 won't wrap readPosition!
                
                bufferData[i] += (delayBuffer.get()->readSample((int)delayInSamples + i) * gainStart);
                localGain += gainIncrementPerSample;
            }
        }
        else
        {
            for (int i = 0; i < buffer.getNumSamples(); ++i)
            {
                bufferData[i] += (delayBuffer.get()->readSample(delayInSamples + (T)i, bufChannel, interpolationType) * gainStart);
                localGain += gainIncrementPerSample;
            }
        }
    }
    
    
    // end of rosen namespace
}
