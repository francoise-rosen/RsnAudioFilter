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


template <typename T>
class CircularBuffer
{
public:
    CircularBuffer();
    
    CircularBuffer(const int& sizeInMs, double sampleRate);
    
    ~CircularBuffer() {}
    
    void setDelayInMs(const T& delayInMs) noexcept;
    
    void write(AudioBuffer<T>& buffer, const int& bufChannel, T gainStart, T gainEnd, bool replace = true) noexcept;
    
    
private:
    unsigned int writePosition;
    unsigned int bufferSize;
    unsigned int channels;
    double currentSampleRate;
    double delayInSamples;
    AudioBuffer<T> circularBuffer;
    
};

template <typename T>
CircularBuffer<T>::CircularBuffer()
:writePosition{0}, bufferSize{0}, channels{0}, currentSampleRate{0}
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
    circularBuffer.setSize(channels, bufferSize);
    circularBuffer.clear();
}

template <typename T>
void CircularBuffer<T>::setDelayInMs(const T& delayInMs) noexcept
{
    delayInSamples = ((double)delayInMs * currentSampleRate) / 1000.0;
}

template <typename T>
void CircularBuffer<T>::write(AudioBuffer<T>& buffer, const int& bufChannel, T gainStart, T gainEnd, bool replace) noexcept
{
    auto* bufferData = buffer.getReadPointer(bufChannel);
    auto localWritePos = writePosition;
}

//==============================================================================
// HELPER FUNCTIONS
//==============================================================================

template <typename T>
T linearInterpolation(T fractionalValue, const T& loValue, const T& hiValue)
{
    T fraction = fractionalValue - (int)fractionalValue;
    return loValue + fraction * (hiValue - loValue);;
}
