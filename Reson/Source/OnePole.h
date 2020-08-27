/*
  ==============================================================================

    Simple OnePole LPF filter for smoothing out control signals
    OnePole.h
    Created: 26 Aug 2020 2:59:45pm
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

namespace sfd {
    
template <typename T>
class OnePole
{
public:
    OnePole() {}
    
    OnePole(const T& freq, double sampleRate)
    :currentSampleRate{sampleRate}
    {
        setFrequency(freq);
    }
    
    ~OnePole() {}
    
    T process(const T& xn)
    {
        T yn = a0 * xn - b1 * z1;
        z1 = yn;
        
        return yn;
    }
    
    void setFrequency(const T& freq)
    {
        assert(currentSampleRate > 0);
        T freqInRad = juce::MathConstants<double>::twoPi * freq / currentSampleRate;
        a0 = sin(freqInRad); // gain
        b1 = a0 - 1.0;         // pole
    }
    
    
private:
    double currentSampleRate;
    T b1 {0};
    T a0 {1.0};
    T z1 {0};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OnePole)
};
    
}
