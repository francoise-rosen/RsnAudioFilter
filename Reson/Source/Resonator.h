/*
  ==============================================================================

    Resonator.h
    Created: 18 Jul 2020 11:08:07am
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace sfd {
    
enum class FilterAlgorithm {BPF, AnalogueMatchedMagnitude, basicReson, symmetricalReson, numOfAlgorithms};


// 2 pole / 2 zero coefficients and state (from biquad algo)
enum filterCoeff {a0, a1, a2, b1, b2, numCoeff};
enum stateZ {x_z1, x_z2, y_z1, y_z2, numStates};

    
// ================================================
// FILTER PARAMETERS
// =================================================
    

template <typename T>
struct FilterParameters
    {
        FilterParameters()
        :frequency{1000}, Q{0.7071}, algorithm{FilterAlgorithm::symmetricalReson}
        {}
        
//        FilterParameters(T frq, T q, FFilterAlgorithms algo)
//        :frequency{frq}, Q{q}, filterAlgorithm{algo}
//        {}
        
        ~FilterParameters() {}
        
        // OR MOVE?
        FilterParameters(const FilterParameters&);
        FilterParameters& operator=(const FilterParameters&);
        
        inline bool operator==(const FilterParameters&);
        inline bool operator!=(const FilterParameters&);
        
        T frequency;
        T Q;
        FilterAlgorithm algorithm;
        static constexpr double Q_MIN = 0.01;
        static constexpr double Q_MAX = 200.0;
    };
    
    template <typename T>
    FilterParameters<T>::FilterParameters(const FilterParameters& params)
    :frequency{params.frequency}, Q{juce::jmax(params.Q, Q_MIN)}, algorithm{params.algorithm}
    {
    }
    
    template <typename T>
    FilterParameters<T>& FilterParameters<T>::operator=(const FilterParameters& params)
    {
        if (*this == params) return *this;
        frequency = params.frequency;
        Q = juce::jmax(params.Q, Q_MIN);
        //Q = params.Q;
        algorithm = params.algorithm;
        return *this;
    }
    
    template <typename T>
    inline bool FilterParameters<T>::operator==(const FilterParameters<T>& params)
    {
        return (frequency == params.frequency && Q == params.Q && algorithm == params.algorithm);
    }
    
    template <typename T>
    inline bool FilterParameters<T>::operator!=(const FilterParameters& params)
    {
        return !(this == params);
    }

// =================================================
// RESONATOR
// =================================================
    
template <typename T>
class Resonator
{
    
public:
    
    Resonator();
    Resonator(const FilterParameters<T>&, const double& sampleRate);
    
    ~Resonator() {}
    
    //==============================================================================
    
    void setParameters(const FilterParameters<T>& params);
    void setSampleRate(const double& sampleRate);
    
    //==============================================================================
    T process(const T& sample) noexcept;
    void reset();
    
private:
    using Math = juce::MathConstants<double>;
    double currentSampleRate;
    FilterParameters<T> filterParameters;
    
    T coeffArray[filterCoeff::numCoeff];
    T stateArray[stateZ::numStates];
    
    void setCoefficients();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Resonator)
};
    
    template <typename T>
    Resonator<T>::Resonator()
    :currentSampleRate{0}
    {
        std::memset(&coeffArray[0], 0, sizeof(T)*numCoeff);
        reset();
    }
    
    template <typename T>
    Resonator<T>::Resonator(const FilterParameters<T>& params, const double& sampleRate)
    :currentSampleRate{sampleRate}, filterParameters{params} 
    {
        assert( (filterParameters.Q >= FilterParameters<T>::Q_MIN) && (filterParameters.Q <= FilterParameters<T>::Q_MAX));
        reset();
        setCoefficients();
    }
    
    
    template <typename T>
    void Resonator<T>::setCoefficients()
    {
        assert(currentSampleRate > 0); // shouldn't call this method before setting the sample rate
        std::memset(&coeffArray[0], 0, sizeof(T)*numCoeff);
        
        T frequency = filterParameters.frequency;
        T Q = filterParameters.Q;
        FilterAlgorithm algorithm = filterParameters.algorithm;
        
        if (algorithm == FilterAlgorithm::symmetricalReson)
        {
            T theta = Math::twoPi * frequency / currentSampleRate;
            T bandwidth = frequency / Q;
            
            coeffArray[b2] = pow(Math::euler, (-2 * Math::twoPi * bandwidth / currentSampleRate));
            coeffArray[b1] = -4 * coeffArray[b2] * cos(theta) / (1 + coeffArray[b2]);
            coeffArray[a0] = 1 - sqrt(coeffArray[b2]);
            coeffArray[a2] = -coeffArray[a0];
            coeffArray[a1] = 0;
            
            return;
        }
        if (algorithm == FilterAlgorithm::basicReson)
        {
            T theta = Math::twoPi * frequency / currentSampleRate;
            T bandwidth = frequency / Q;
            coeffArray[b2] = pow(Math::euler, (-2 * Math::twoPi * bandwidth / currentSampleRate));
            coeffArray[b1] = -4 * coeffArray[b2] * cos(theta) / (1 + coeffArray[b2]);
            coeffArray[a0] = (1 - coeffArray[b2]) * sqrt(1 - coeffArray[b1] * coeffArray[b1] / (4 * coeffArray[b2]));
            
            coeffArray[a1] = 0;
            coeffArray[a2] = 0;
            
            return;
        }
        
        if (algorithm == FilterAlgorithm::BPF)
        {
            T K = tan(Math::pi * frequency / currentSampleRate);
            T rDelta = 1 / (K * K * Q + K + Q);
            
            coeffArray[a0] = K * rDelta;
            coeffArray[a1] = 0;
            coeffArray[a2] = -K * rDelta;
            coeffArray[b1] = (2 * Q * (K * K - 1)) * rDelta;
            coeffArray[b2] = (K * K * Q - K + Q) * rDelta;
            
            return;
        }
        
        if (algorithm == FilterAlgorithm::AnalogueMatchedMagnitude)
        {
            // Vicanek's BPF Loose Fit
            T omega_c = Math::twoPi * frequency /  currentSampleRate;
            T q = 0.5 / Q;
            T exp_c = pow(Math::euler, - (q * omega_c));
            T f0 = omega_c / Math::pi;
            
            coeffArray[b1] = (q <= 1) ? (-2 * exp_c * cos(omega_c * sqrt(1 - q * q))) : (-2 * exp_c * cosh(omega_c * sqrt(q * q - 1)));
            coeffArray[b2] = pow(Math::euler, - (2 * q * omega_c));
            
            T r0 = (1 + coeffArray[b1] + coeffArray[b2]) / omega_c * Q;
            T r1 = (1 - coeffArray[b1] + coeffArray[b2]) * (f0 / Q) / sqrt((1 - f0 * f0) * (1 - f0 * f0) + f0 * f0 / (Q * Q));
            
            coeffArray[a1] = - r1 / 2;
            coeffArray[a0] = (r0 - coeffArray[a1]) / 2;
            coeffArray[a2] = -coeffArray[a0] - coeffArray[a1];
            
            return;
        }
    }
    
    template <typename T>
    T Resonator<T>::process(const T& xn) noexcept
    {
        // biquad Direct II topology
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
    void Resonator<T>::setParameters(const FilterParameters<T>& params)
    {
        
        filterParameters = params;
        setCoefficients();
    }
    
    template <typename T>
    void Resonator<T>::reset()
    {
        std::memset(&stateArray[0], 0, sizeof(T)*numStates);
    }

    
}


// Resonator Bank (check DynKlank)
