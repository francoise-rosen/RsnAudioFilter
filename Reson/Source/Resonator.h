/*
  ==============================================================================

    Resonator.h
    Created: 18 Jul 2020 11:08:07am
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

enum class biquadAlgorithm {basicReson, symmetricalReson, numOfAlgorithms};


// 2 pole / 2 zero coefficients and state (from biquad algo)
enum filterCoeff {a0, a1, a2, b1, b2, numCoeff};
enum stateZ {x_z1, x_z2, y_z1, y_z2, numStates};

template <typename T>
class Reson
{
    
public:
    
    Reson(biquadAlgorithm algo = biquadAlgorithm::symmetricalReson);
    Reson(const T& freq, const T& q, double sampleRate, biquadAlgorithm algo = biquadAlgorithm::symmetricalReson);
    
    ~Reson();
    
    //==============================================================================
    void setCoeff();
    void setCutOff(const T& cutOffValue);
    void setQ(const T& q);
    void setAlgorithm(const biquadAlgorithm&);
    
    //==============================================================================
    T getCutOff() const {return cutoff;}
    T getQ() const {return qualityFactor;}
    biquadAlgorithm getAlgorithm() const {return algorithm;}
    
    //==============================================================================
    T process(const T& sample) noexcept;
    void reset();
    
private:
    using Math = juce::MathConstants<double>;
    T cutoff;
    T qualityFactor;
    biquadAlgorithm algorithm;
    T coeffArray[filterCoeff::numCoeff];
    T stateArray[stateZ::numStates];
    double currentSampleRate;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Reson)
};


// Resonator Bank (check DynKlank)
