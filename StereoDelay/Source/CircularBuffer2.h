/*
  ==============================================================================

    CircularBuffer2.h
    Created: 25 Jun 2020 2:24:26pm
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

// Just one circular buffer / delay class
// one write position but multiple read positions (taps)
// 7 types of interpolation (reading)

/*
 
 Members
 - AudioBuffer
 - writePosition
 - bufferSize
 - channels
 - currentSampleRate
 - int numOfTaps
 - double delayInSamples - std::vector<double> delayInSamples(numOfTaps)
 
 */
enum class Interpolation
{
    none, linear, cosine, cubic, lagrange, spline
    
};

//==============================================================================
// HELPER FUNCTIONS
//==============================================================================

template <typename T>
T linearInterpolation(T fraction, const T& loValue, const T& hiValue)
{
    return loValue + fraction * (hiValue - loValue);;
}


template <typename T>
class CircularBuffer
{
public:
    CircularBuffer();
    
    CircularBuffer(const int& sizeInMs, double sampleRate);
    
    ~CircularBuffer() {}
    
    forcedinline void setDelayInMs(const T& delayInMs) noexcept;
    
    void writeBlock(AudioBuffer<T>& buffer, const int& bufChannel, T gainStart, T gainEnd, bool replace = true) noexcept;
    
    void readBlock(AudioBuffer<T>& buffer, const int& bufChannel, const T& delayInMs, const T& gainStart, const T& gainEnd, Interpolation type = Interpolation::none) noexcept;
    
    void advanceWrite(const int& numSamples);
    
    
private:
    unsigned int writePosition;
    unsigned int bufferSize;
    unsigned int wrapMask;
    unsigned int channels;
    double currentSampleRate;
    double delayInSamples;
    AudioBuffer<T> circularBuffer;
    
};

template <typename T>
CircularBuffer<T>::CircularBuffer()
:writePosition{0}, bufferSize{0}, channels{0}, currentSampleRate{0}, wrapMask{0}
{
    circularBuffer.setSize(channels, bufferSize);
    circularBuffer.clear();
}

template <typename T>
CircularBuffer<T>::CircularBuffer(const int& sizeInMs, double sampleRate)
:writePosition{0}, channels{1}, currentSampleRate{sampleRate}
{
    double requestedSizeInSamples = ((double)sizeInMs / 1000.0) * sampleRate;
    // adjusting the size to binary
    bufferSize = (unsigned)(pow(2, ceil(log2(requestedSizeInSamples))));
    wrapMask = bufferSize - 1;
    circularBuffer.setSize(channels, bufferSize);
    circularBuffer.clear();
}

template <typename T>
void CircularBuffer<T>::setDelayInMs(const T& delayInMs) noexcept
{
    delayInSamples = (delayInMs * currentSampleRate) / 1000.0;
}

template <typename T>
void CircularBuffer<T>::writeBlock(AudioBuffer<T>& buffer, const int& bufChannel, T gainStart, T gainEnd, bool replace) noexcept
{
    auto* bufferData = buffer.getReadPointer(bufChannel);
    auto* circularBufferData = circularBuffer.getWritePointer(0);
    auto localWritePos = writePosition;
    
    T gainIncrement = (gainEnd - gainStart) / buffer.getNumSamples();
    auto localGain = gainStart;
    
    // replace = direct signal, overrides samples
    if (replace)
    {
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            
            circularBufferData[localWritePos] = bufferData[i] * localGain;
            localGain += gainIncrement;
            ++localWritePos;
            localWritePos &= wrapMask;
        }
    }
    else
    {
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            circularBufferData[localWritePos] += bufferData[i] * localGain;
            localGain += gainIncrement;
            ++localWritePos;
            localWritePos &= wrapMask;
        }
    }
}

// read
template <typename T>
void CircularBuffer<T>::readBlock(AudioBuffer<T>& buffer, const int& bufChannel, const T& delayInMs, const T& gainStart, const T& gainEnd, Interpolation type) noexcept
{
    // get the write pointer to the buffer, and the read to the cicrularBuffer
    auto* bufferData = buffer.getWritePointer(bufChannel);
    auto* circularBufferData = circularBuffer.getReadPointer(0);
    
    // set the delay in samples value and the initial read position
    setDelayInMs(delayInMs);
    int readPosition = writePosition - (int)delayInSamples;
    if (readPosition < 0) readPosition += bufferSize;
    
    // gain ramp
    auto localGain = gainStart;
    T gainIncrement = (gainEnd - gainStart) / (T)buffer.getNumSamples();
    
    // Interpolation cases
    if (type == Interpolation::none)
    {
       
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            bufferData[i] += circularBufferData[readPosition] * localGain;
           // bufferData[i] += circularBufferData[readPosition];
            ++readPosition;
            localGain += gainIncrement;
            readPosition &= wrapMask;
        }
    }
    
    else if (type == Interpolation::linear)
    {
        T fraction = delayInSamples - (int)delayInSamples;
        
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            auto loValue = circularBufferData[readPosition];
            unsigned int upperIndex = (int)readPosition + 1;
            upperIndex &= wrapMask;
            T hiValue = circularBufferData[upperIndex];
            auto sample = linearInterpolation<T>(fraction, loValue, hiValue);
            bufferData[i] += sample * localGain;
            ++readPosition;
            readPosition &= wrapMask;
            localGain += gainIncrement;
        }
    }
        
}

template <typename T>
void CircularBuffer<T>::advanceWrite(const int& numSamples)
{
    writePosition += numSamples;
    if (writePosition > bufferSize - 1) writePosition -= bufferSize;
}



