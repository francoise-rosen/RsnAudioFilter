/*
  ==============================================================================

    Resonator.cpp
    Created: 18 Jul 2020 11:08:07am
    Author:  syfo_dias

  ==============================================================================
*/

#include "Resonator.h"

template <typename T>
Reson<T>::Reson(biquadAlgorithm algo)
:cutoff{1000}, qualityFactor{1}, currentSampleRate{0}, algorithm{algo}
{
    reset();
    setCoeff();
}

template <typename T>
Reson<T>::Reson(const T& freq, const T& q, double sampleRate, biquadAlgorithm algo)
:cutoff{freq}, currentSampleRate{sampleRate}, algorithm{algo}
{
    assert( q > (T)0.001 && q < 200.0); // check this!
    reset();
    setCoeff();
}

template <typename T>
Reson<T>::~Reson() {}

template <typename T>
void Reson<T>::setCoeff()
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
T Reson<T>::process(const T& xn) noexcept
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
void Reson<T>::setCutOff(const T& freq)
{
    cutoff = freq;
    setCoeff();
}

template <typename T>
void Reson<T>::setQ(const T &q)
{
    qualityFactor = q;
    setCoeff();
}

template <typename T>
void Reson<T>::reset()
{
    for(int i = 0; i  < stateZ::numStates; ++i)
        stateArray[i] = 0;
        
}
