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
 
 */

enum Interpolation
{
    none, linear, cubic, lagrange, spline
    
};

// this is a simple DDL. Only 1 channel buffer here
// T is a float or double
template <typename T>
class CircularBuffer
{
public:
    
    CircularBuffer()
    :bufferSize{1 << 19}, writePosition{0}, channels{1}
    {
        circularBuffer = std::make_unique<AudioBuffer<T>>(channels, bufferSize);
        circularBuffer.get()->clear(); // clear the buffer and set all values to 0
    }
    
    void getBufferSize() const {return bufferSize;}
    void clearBuffer() {circularBuffer.get()->clear();}
    
    void writeBuffer(AudioBuffer<T>& buffer, const int& channel, T gainStart, T gainEnd, bool replace);
    void
    //void writeSample(const T& sample); //write a single sample into a circularBuffer
    void advanceWritePosition(const int& numSamples);
    //T readBuffer(int delayInSamples);
    T readBuffer(double delayFraction, int delayBufferChannel, Interpolation = Interpolation::none);
    
private:
    unsigned int bufferSize;
    unsigned int writePosition;
    int channels;
    std::unique_ptr<AudioBuffer<T>> circularBuffer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CircularBuffer)
};

// rewrite this as sample-wise
template <typename T>
void CircularBuffer<T>::writeBuffer(AudioBuffer<T>& buffer, const int& channel, T gainStart, T gainEnd, bool replace)
{
    auto* bufferData = buffer.getReadPointer(channel);
    const int samplesLeft = bufferSize - writePosition;
    
    if (buffer.getNumSamples() <= samplesLeft)
    {
        if (replace)
        {
            circularBuffer.get()->copyFromWithRamp(0, writePosition, bufferData, buffer.getNumSamples(), gainStart, gainEnd);
        }
        else
            circularBuffer.get()->addFromWithRamp(0, writePosition, bufferData, buffer.getNumSamples(), gainStart, gainEnd);
    }
    
    else
    {
        auto crossPos = samplesLeft / buffer.getNumSamples();
        auto crossGain = jmap(static_cast<float>(crossPos), gainStart, gainEnd);
        
        if (replace)
        {
            circularBuffer.get()->copyFromWithRamp(0, writePosition, bufferData, samplesLeft, gainStart, crossGain);
            circularBuffer.get()->copyFromWithRamp(0, 0, bufferData + samplesLeft, crossGain, gainEnd);
        }
        else
        {
            circularBuffer.get()->addFromWithRamp(0, writePosition, bufferData, samplesLeft, gainStart, crossGain);
            circularBuffer.get()->addFromWithRamp(0, 0, bufferData + samplesLeft, crossGain, gainEnd);
        }
    }
    
}

// this is not really a safe way. What if we make mistake in numSamples?
// of forgot to add it in the end of the processing block?


// write position is shared among the channels, what about read?
template <typename T>
void CircularBuffer<T>::advanceWritePosition(const int& numSamples)
{
    writePosition += numSamples;
    if (writePosition > bufferSize) writePosition -= bufferSize;
}
