/*
  ==============================================================================

    Simple OnePole LPF filter for smoothing out control signals
    OnePole.h
    Created: 26 Aug 2020 2:59:45pm
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once


template <typename T>
class OnePole
{
    OnePole() {}
    ~OnePole() {}
    
    OnePole(const T& freq, double sampleRate)
    :currentSampleRate{sampleRate}
    {
        setFrequency(freq);
    }
    
    T process(const T& input)
    {
        T xn = input;
        T z1 = 
    }
    
    void setFrequency(const T& freq)
    {
        assert(currentSampleRate > 0);
        a0 = b1 = 0;
        T freqInRad = juce::MathConstants<double>::twoPi * freq / currentSampleRate;
        a0 = sin(freqInRad); // gain
        b1 = a0 - 1;         // pole
    }
    

public:
    double currentSampleRate;
    T b1;
    T a0;
};
