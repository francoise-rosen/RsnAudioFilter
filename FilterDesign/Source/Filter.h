/*
  ==============================================================================

    FilterDesign.h
    Created: 23 Jul 2020 4:31:07pm
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once
#include "Biquad.h"

// this filter object must be able to host several biquads
// up to 6-10th order
// for instance 3rd order ButterLPF with have 1 2nd Order ButterLPF Biquad followed
// by 1st order ButterLPF Biquad

// this version has no peak filter!

// for delay plugin use only butter and lo / hi shelves

// This version: FilterDesign(algorithm::LPF, order=4, frequency=2000, Q=0.707, boost = 0dB, dryWet=100%)
// Next version: FilterDesign(int numOfBands=5, FilterType algorithms[numOfBands], orderOfEveryBand, frequencies, q's, dryWet=100%, int bandProcessingType[numOfBand](MS or stereo)) <- cascade filter as prototype for Matrix (Morph) Filter.
// Employ


namespace rosen

{
    
    using Math = juce::MathConstants<double>;
    
    enum class FilterType {
        LPF6, HPF6, LPF12, HPF12, ButterLPF6, ButterHPF6, ButterLPF12, ButterHPF12, ButterBPF6, BPF, BSF, LoShelf,
        HiShelf, Peak, NumOfTypes
    };
    
    template <typename T>
    T scale(T& valueToScale, const T inMin, const T inMax, const T outMin, const T outMax)
    {
        jassert(inMin < inMax);
        if(outMin == outMax) return outMin;
        const T factor = (outMax - outMin) / (inMax - inMin);
        return factor * (valueToScale - inMin) + outMin;
    }

    //==============================================================================
    template <typename Type>
    struct FilterParameters
    {
        FilterParameters() {}
        
        // use move instead of copy in the future
        FilterParameters& operator=(const FilterParameters& params);
        inline bool operator==(const FilterParameters& params);
        inline bool operator!=(const FilterParameters& params);
        
        FilterType algorithm = FilterType::LPF6;
        Type freq = 100.0;
        Type Q = 0.7071; // that would be disabled for Butterworth
        Type boost = 0; // in which units?
        //int order = 2; // default to 2
    };
    
    template <typename Type>
    FilterParameters<Type>& FilterParameters<Type>::operator=(const FilterParameters& params)
    {
        algorithm = params.algorithm;
        freq = params.freq;
        Q = params.Q;
        boost = params.boost;
        
        return *this;
    }
    
    template <typename Type>
    inline bool FilterParameters<Type>::operator==(const FilterParameters& params)
    {
        return ((algorithm == params.algorithm) && (freq == params.freq) && (Q == params.Q) && (boost == params.boost));
    }
    
    template <typename Type>
    inline bool FilterParameters<Type>::operator!=(const FilterParameters& params)
    {
        return !(this == params);
    }
    
    
    //==============================================================================
    template <typename Type>
    class Filter
    {
    public:
        Filter();
        Filter(FilterParameters<Type>& params, double sampleRate);
        ~Filter();
        
        Type process(const Type& sample) noexcept;
        void resetAll(); // reset all biquads and state
        
        // setters
        void setSampleRate(double sampleRate);
        void setParameters(const FilterParameters<Type>& params);
        
    private:
   
        FilterParameters<Type> parameters;
        double currentSampleRate;
        
        // 1-2 order
        std::unique_ptr<rosen::Biquad<Type>> biquad;
    
        // will determine the number of biquads to use in process
        Type coefficients[numCoeffs];
        void computeCoefficients();
        
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Filter)
    };
    
    template <typename Type>
    Filter<Type>::Filter()
    :currentSampleRate{0}
    {
        biquad = std::make_unique<rosen::Biquad<Type>>(); // use default biquad
    }
    
    template <typename Type>
    Filter<Type>::Filter(FilterParameters<Type>& params, double sampleRate)
    :parameters{params}, currentSampleRate{sampleRate}
    {
        biquad = std::make_unique<rosen::Biquad<Type>>();
        computeCoefficients();
    }
    
    template <typename Type>
    Filter<Type>::~Filter()
    {

    }
    
    template <typename Type>
    Type Filter<Type>::process(const Type& sample) noexcept
    {
        Type outputSample = coefficients[c0] * sample + coefficients[d0] * biquad->process(sample);
        //Type outputSample = 0 * sample + 1 * biquad->process(sample);
        return outputSample;
    }
    
    template <typename Type>
    void Filter<Type>::computeCoefficients()
    {
        assert (currentSampleRate > 0);
        // reset coeff
        std::fill(coefficients, coefficients + numCoeffs, 0);
        coefficients[d0] = 1;
        
        // extract the parameters
        FilterType algorithm = parameters.algorithm;
        double freq = parameters.freq;
        double q = parameters.Q;
        double boost = parameters.boost;
        
        if (algorithm == FilterType::LPF6)
        {
            double theta = Math::twoPi * freq / currentSampleRate;
            double gamma = cos (theta) / (1.0 + sin(theta));
            
            coefficients[a0] = (1.0 - gamma) * 0.5;
            coefficients[a1] = coefficients[a0];
            coefficients[a2] = 0.0;
            coefficients[b1] = - gamma;
            coefficients[b2] = 0.0;
            
            biquad->setCoefficients(coefficients);
            return;
    
        }
        
        if (algorithm == FilterType::HPF6)
        {
            double theta = Math::twoPi * freq / currentSampleRate;
            double gamma = cos (theta) / (1.0 + sin(theta));
            
            coefficients[a0] = (1 + gamma) * 0.5;
            coefficients[a1] = - (1 + gamma) * 0.5;
            coefficients[a2] = 0.0;
            coefficients[b1] = - gamma;
            coefficients[b2] = 0.0;
            
            biquad->setCoefficients(coefficients);
            return;
        }
        
        if (algorithm == FilterType::LPF12)
        {
            Type theta = Math::twoPi * freq / currentSampleRate;
            if (theta >= Math::pi) return;
            
            Type d = 1 / q;
            
            Type beta = 0.5 * (1 - sin(theta) * d * 0.5) / (1 + sin(theta) * d * 0.5);
            Type gamma = (0.5 + beta) * cos(theta);
            
            coefficients[a0] = (0.5 + beta - gamma) * 0.5;
            coefficients[a1] = 0.5 + beta - gamma;
            coefficients[a2] = coefficients[a0];
            coefficients[b1] = -2 * gamma;
            coefficients[b2] = 2 * beta;
            
            biquad->setCoefficients(coefficients);
            return;
        }
        
        if (algorithm == FilterType::HPF12)
        {
            Type theta = Math::twoPi * freq / currentSampleRate;
            if (theta >= Math::pi) return;
            
            Type d = 1 / q;
            Type beta = 0.5 * (1 - sin(theta) * d * 0.5) / (1 + sin(theta) * d * 0.5);
            Type gamma = (0.5 + beta) * cos(theta);
            
            coefficients[a0] = (0.5 + beta + gamma) * 0.5;
            coefficients[a1] = -(0.5 + beta + gamma);
            coefficients[a2] = coefficients[a0];
            coefficients[b1] = -2 * gamma;
            coefficients[b2] = 2 * beta;
            
            biquad->setCoefficients(coefficients);
            return;
        }
        
        if (algorithm == FilterType::ButterLPF6)
        {
            Type tArg = juce::jmin(Math::pi * freq / currentSampleRate, Math::halfPi * 0.95);
            Type omega_a = tan(tArg);
            
            coefficients[a0] = omega_a / (omega_a + 1);
            coefficients[a1] = coefficients[a0];
            coefficients[a2] = 0.0;
            coefficients[b1] = (omega_a - 1) / (omega_a + 1);
            coefficients[b2] = 0.0;
            
            biquad->setCoefficients(coefficients);
            return;
        }
        
        if (algorithm == FilterType::ButterHPF6)
        {
            Type tArg = juce::jmin(Math::pi * freq / currentSampleRate, Math::halfPi * 0.95);
            Type omega_a = tan(tArg);
            
            coefficients[a0] = 1 / (omega_a + 1);
            coefficients[a1] = - coefficients[a0];
            coefficients[a2] = 0.0;
            coefficients[b1] = (omega_a - 1) / (omega_a + 1);
            coefficients[b2] = 0.0;
            
            biquad->setCoefficients(coefficients);
            return;
        }
        
        if (algorithm == FilterType::ButterLPF12)
        {
            Type tArg = Math::pi * freq / currentSampleRate;
            if (tArg > Math::halfPi * 0.95) tArg = Math::halfPi * 0.95;
            Type C = 1 / tan(tArg);
            
            // nominator
            coefficients[a0] = 1 / (1 + sqrt(2.0) * C + C * C);
            coefficients[a1] = 2 * coefficients[a0];
            coefficients[a2] = coefficients[a0];
            // denominator
            coefficients[b1] = 2 * coefficients[a0] * (1 - C * C);
            coefficients[b2] = coefficients[a0] * (1 - sqrt(2) * C + C * C);
            
            biquad->setCoefficients(coefficients);
            return;
        }
        
        // gains 2 dB! when freq is low
        if (algorithm == FilterType::ButterHPF12)
        {
            Type tArg = Math::pi * freq / currentSampleRate;
            if (tArg > Math::halfPi * 0.95) tArg = Math::halfPi * 0.95;
            Type C = tan(tArg);
            
            coefficients[a0] = 1 / (1 + sqrt(2.0) * C + C * C);
            coefficients[a1] = - 2 * coefficients[a0];
            coefficients[a2] = coefficients[a0];
            
            // denominator
            coefficients[b1] = 2 * coefficients[a0] * (C * C - 1);
            coefficients[b2] = coefficients[a0] * (1 - sqrt(2.0) * C + C * C);
            
            biquad->setCoefficients(coefficients);
            return;
            
        }
        
        // blows up!
        if (algorithm == FilterType::ButterBPF6)
        {
            Type local_Q = scale(q, 0.02, 40.0, 1.0, freq);
            Type tArg = Math::pi * freq / currentSampleRate;
            if (tArg > Math::halfPi * 0.95) tArg = Math::halfPi * 0.95;
            Type omega_o = tan(tArg);
            //Type BW = freq / local_Q;
            
            Type D = local_Q + local_Q * omega_o + omega_o;
            coefficients[a0] = omega_o / D;
            coefficients[a1] = 1 / D;
            coefficients[a2] = 0.0;
            coefficients[b1] = - (local_Q + local_Q * omega_o + 1) * coefficients[a1];
            coefficients[b2] = 0.0;
            
            biquad->setCoefficients(coefficients);
            return;
        }
        
    }
    
    template <typename Type>
    void Filter<Type>::setSampleRate(double sampleRate)
    {
        if (currentSampleRate == sampleRate) return;
        currentSampleRate = sampleRate;
        computeCoefficients();
    }
    
    template <typename Type>
    void Filter<Type>::resetAll()
    {
        // clear all arrays
        // reset biquads
        memset(&coefficients[0], 0, sizeof(Type)*numCoeffs);
        biquad->reset();
    }
    
    template <typename Type>
    void Filter<Type>::setParameters(const FilterParameters<Type>& params)
    {
        if (parameters == params) return;
        parameters = params;
        if (parameters.Q <= 0) parameters.Q = 0.7071;
        computeCoefficients();
    }
}
