/*
  ==============================================================================

    Delay.h
    Created: 16 Jun 2020 11:05:40am
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

// Posible I/O layouts:
// Mono -> Mono
// Mono -> Stereo (Single Delay)
// Mono -> Stereo (L and R delays)
// Stereo -> Stereo (single delay)
// Stereo - < Stereo (L and R delays)

// Simple DDL. Only one channel z-D with feedbacl
// enum for interpolation

enum class Interpolation
{
    none, linear
};

// CircularBuffer class, as one channel DDL
// Delay class (Stereo (2 DDL), MONO (one DLL), PingPong(2 DLL))


// this is a simple DDL. Only 1 channel buffer here
// T is a float or double
template <typename T>
class CircularBuffer
{
public:
    
    CircularBuffer()
    :bufferSize{1 << 19}, writePosition{0}
    {
        circularBuffer = std::make_unique<AudioBuffer<T>>(1, bufferSize);
        circularBuffer.get()->clear(); // clear the buffer and set all values to 0
    }
    
    void getBufferSize() const {return bufferSize;}
    void clearBuffer() {circularBuffer.get()->clear();}
    
    void writeBuffer(AudioBuffer<T>& buffer, const int& channel, T gainStart, T gainEnd, bool replace);
    void writeSample(const T& sample); //write a single sample into a circularBuffer
    void advanceWritePosition(const int& numSamples);
    T readBuffer(int delayInSamples);
    T readBuffer(double delayFraction, Interpolation = Interpolation::none);
    
private:
    unsigned int bufferSize;
    unsigned int writePosition;
    std::unique_ptr<AudioBuffer<T>> circularBuffer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CircularBuffer)
};

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

template <typename T>
void CircularBuffer<T>::advanceWritePosition(const int& numSamples)
{
    writePosition += numSamples;
    if (writePosition > bufferSize) writePosition -= bufferSize;
}


// DELAY VERSION 1 - COPY AND ADD AUDIOBUFFERS
template <typename T>
class Delay
{
public:
    Delay(const int& sz = 1 << 16, double sampleRate = 0)
    :writePosition{0}, currentSampleRate{sampleRate}
    {
        delayBuffer.setSize(1, sz);
        delayBuffer.clear();
        setDelayInMs(200); // this will initialise delayInSamples and readPosition
        std::cout << "delay line initialised\n";
    }
    
    ~Delay() {}
    
    // max delay size
    void setBufferSize(const int& sz)
    {
        delayBuffer.setSize(1, sz);
        delayBuffer.clear();
    }
    
    // call this in the beginning of AudioProcessor::processBlock()
    // to update readPosition
    void setDelayInMs(const T& delayInMs)
    {
        delayInSamples = roundToInt(delayInMs * currentSampleRate / 1000.0);
        readPosition = writePosition - delayInSamples;
        if (readPosition < 0) readPosition += delayBuffer.getNumSamples();
        
    }
    
    int delayLineLength() const {return delayBuffer.getNumSamples();}
    
    void clearBuffer()
    {
        delayBuffer.clear();
    }
    
    void reset(double sampleRate)
    {
        if (currentSampleRate == sampleRate)
        {
            delayBuffer.clear();
            return;
        }
        
    }
    
    // num of tap
    
    // tap delay times
    
    // write
    void writeToBuffer(AudioBuffer<T>& buffer, const int& inputChannel, T gainStart, T gainEnd, bool replace) noexcept;
    
    // read
    void readFromBuffer(AudioBuffer<T>& buffer, const int& outputChannel, T gainStart, T gainEnds) noexcept;
    
    // call this in the end of AudioProcessor::processBlock()
    void advanceWritePosition(const int& numSamples)
    {
        writePosition += numSamples;
        if (writePosition > delayBuffer.getNumSamples() - 1) writePosition -= delayBuffer.getNumSamples();
    }
    
    void printData()
    {
        std::cout << "writePosition: " << writePosition
        << "\nreadPosition: " << readPosition
        << "\ndelayBufferSize " << delayBuffer.getNumSamples()
        << "\ndelayInSamples " << delayInSamples
        << "\ncurrentSampleRate " << currentSampleRate << '\n';
    }
                        
private:
    //CircularBuffer<T> delayBuffer; // one per delay object
    int writePosition;
    int readPosition;
    AudioBuffer<T> delayBuffer;
    int delayInSamples;
    int numOfTaps = 1;
    double currentSampleRate = 0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Delay)
};


template <typename T>
void Delay<T>::writeToBuffer(AudioBuffer<T>& buffer, const int& inputChannel, T gainStart, T gainEnd, bool replace) noexcept
{
    auto* bufferData = buffer.getReadPointer(inputChannel);
    auto samplesLeft = delayBuffer.getNumSamples() - writePosition;
    
    if (samplesLeft >= buffer.getNumSamples())
    {
        if (replace)
        {
        delayBuffer.copyFromWithRamp(0, writePosition, bufferData, buffer.getNumSamples(), gainStart, gainEnd);
        }
        else {
            delayBuffer.addFromWithRamp(0, writePosition, bufferData, buffer.getNumSamples(), gainStart, gainEnd);
        }
    }
    else
    {
        auto crossPos = static_cast<float>(samplesLeft) / buffer.getNumSamples();
        auto crossGain = jmap(crossPos, gainStart, gainEnd);
        
        if (replace)
        {
            delayBuffer.copyFromWithRamp(0, writePosition, bufferData, samplesLeft, gainStart, crossGain);
            delayBuffer.copyFromWithRamp(0, 0, bufferData + samplesLeft, buffer.getNumSamples() - samplesLeft, crossGain, gainEnd);
        }
        
        else
        {
            delayBuffer.addFromWithRamp(0, writePosition, bufferData, samplesLeft, gainStart, crossGain);
            delayBuffer.addFromWithRamp(0, 0, bufferData + samplesLeft, buffer.getNumSamples() - samplesLeft, crossGain, gainEnd);
        }
    }
}

template <typename T>
void Delay<T>::readFromBuffer(AudioBuffer<T>& buffer, const int& outputChannel, T gainStart, T gainEnd) noexcept
{
    auto* delayBufferData = delayBuffer.getReadPointer(0);
    auto samplesLeft = delayBuffer.getNumSamples() - readPosition;
    
    // we have enough samples to read before the end of the delayBuffer
    if (samplesLeft >= buffer.getNumSamples())
    {
        buffer.addFromWithRamp(outputChannel, 0, delayBufferData + readPosition, buffer.getNumSamples(), gainStart, gainEnd);
    }
    
    else
    {
        auto crossPos = static_cast<float>(samplesLeft) / buffer.getNumSamples();
        auto crossGain = jmap(crossPos, gainStart, gainEnd);
        buffer.addFromWithRamp(outputChannel, 0, delayBufferData + readPosition, samplesLeft, gainStart, crossGain);
        buffer.addFromWithRamp(outputChannel, samplesLeft, delayBufferData, buffer.getNumSamples() - samplesLeft, crossGain, gainEnd);
    }
}
        
                             
                             
                    
                        
                        
