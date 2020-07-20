/*
  ==============================================================================

    Resonator.h
    Created: 18 Jul 2020 11:08:07am
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace rosen {
    
enum class biquadAlgorithm {basicReson, symmetricalReson, numOfAlgorithms};


// 2 pole / 2 zero coefficients and state (from biquad algo)
enum filterCoeff {a0, a1, a2, b1, b2, numCoeff};
enum stateZ {x_z1, x_z2, y_z1, y_z2, numStates};

template <typename T>
class Resonator
{
    
public:
    
    Resonator(biquadAlgorithm algo = biquadAlgorithm::symmetricalReson);
    Resonator(const T& freq, const T& q, double sampleRate, biquadAlgorithm algo = biquadAlgorithm::symmetricalReson);
    
    ~Resonator() {}
    
    //==============================================================================
    
    void setFreq(const T& cutOffValue);
    void setQ(const T& q);
    void setAlgorithm(const int&);
    
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
    double currentSampleRate;
    biquadAlgorithm algorithm;
    T coeffArray[filterCoeff::numCoeff];
    T stateArray[stateZ::numStates];
    
    void setCoeff();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Resonator)
};
    
    template <typename T>
    Resonator<T>::Resonator(biquadAlgorithm algo)
    :cutoff{1000}, qualityFactor{1}, currentSampleRate{0}, algorithm{algo}
    {
        reset();
        setCoeff();
    }
    
    template <typename T>
    Resonator<T>::Resonator(const T& freq, const T& q, double sampleRate, biquadAlgorithm algo)
    :cutoff{freq}, currentSampleRate{sampleRate}, algorithm{algo}
    {
        assert( q > (T)0.001 && q < 200.0); // check this!
        reset();
        setCoeff();
    }
    
    
    template <typename T>
    void Resonator<T>::setCoeff()
    {
        if (algorithm == biquadAlgorithm::symmetricalReson)
        {
            T theta = Math::twoPi * cutoff / currentSampleRate;
            T bandwidth = cutoff / qualityFactor;
            coeffArray[b2] = pow(Math::euler, (-2 * Math::twoPi * bandwidth / currentSampleRate));
            coeffArray[b1] = -4 * coeffArray[b2] * cos(theta) / (1 + coeffArray[b2]);
            coeffArray[a0] = 1 - sqrt(coeffArray[b2]);
            coeffArray[a2] = -coeffArray[a0];
            coeffArray[a1] = 0;
        }
        else if (algorithm == biquadAlgorithm::basicReson)
        {
            T theta = Math::twoPi * cutoff / currentSampleRate;
            T bandwidth = cutoff / qualityFactor;
            coeffArray[b2] = pow(Math::euler, (-2 * Math::twoPi * bandwidth / currentSampleRate));
            coeffArray[b1] = -4 * coeffArray[b2] * cos(theta) / (1 + coeffArray[b2]);
            coeffArray[a0] = (1 - coeffArray[b2]) * sqrt(1 - coeffArray[b1] * coeffArray[b1] / (4 * coeffArray[b2]));
            
            coeffArray[a1] = 0;
            coeffArray[a2] = 0;
        }
    }
    
    template <typename T>
    T Resonator<T>::process(const T& xn) noexcept
    {
        // biquad topology
        // y(n) = a0 * x(n) + a1 * x(n - 1) + a2 * x(n - 2) - b1 * y(n - 1) - b2 * y(n - 2);
        T yn = coeffArray[a0] * xn + coeffArray[a1] * stateArray[x_z1] + coeffArray[a2] * stateArray[x_z2] - coeffArray[b1] * stateArray[y_z1] - coeffArray[b2] * stateArray[y_z2];
        
        // update registers
        stateArray[x_z2] = stateArray[x_z1];
        stateArray[x_z1] = xn;
        stateArray[y_z2] = stateArray[y_z1];
        stateArray[y_z1] = yn;
        
        return yn;
    }
    
    template <typename T>
    void Resonator<T>::setFreq(const T& freq)
    {
        cutoff = freq;
        setCoeff();
    }
    
    template <typename T>
    void Resonator<T>::setQ(const T &q)
    {
        qualityFactor = q;
        setCoeff();
    }
    
    template <typename T>
    void Resonator<T>::reset()
    {
        for(int i = 0; i  < stateZ::numStates; ++i)
            stateArray[i] = 0;
        
    }
    
    template <typename T>
    void Resonator<T>::setAlgorithm(const int& algo)
    {
        algorithm = static_cast<biquadAlgorithm>(algo);
    }
    
}


// Resonator Bank (check DynKlank)
