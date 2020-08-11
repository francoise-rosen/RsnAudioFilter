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
    
    enum class FilterType {
        LPF, HPF, BPF, BSF, ButterLPF, ButterHPF, LoShelf,
        HiShelf, Peak, NumOfTypes
    };
    
    // 6th order max here
    enum gains {c0, d0, c1, d1, c2, d2, maxOrder};
    
    //enum filterCoeff {a0, a1, a2, b1, b2, c, d, totalCoeff};
    
    enum class rollOff {db6 = 1, db12, db18, db24, db30, db36,
        db48 = 8, db60 = 10, db72 = 12};
    
    template <typename Type>
    struct FilterParameters
    {
        FilterParameters() {}
        FilterParameters& operator=(const FilterParameters& params);
        
        FilterType algorithm = FilterType::LPF;
        Type freq = 100.0;
        Type Q = 0.707; // that would be disabled for Butterworth
        Type boost = 0; // in which units?
        int order = 2; // default to 2
    };
    
    template <typename Type>
    class Filter
    {
    public:
        Filter();
        Filter(FilterParameters<Type>& params);
        ~Filter();
        
        Type process(const Type& sample) noexcept;
        void resetAll(); // reset all biquads and state
        
    private:
   
        FilterParameters<Type> parameters;
        
        // 1-2 order
        std::unique_ptr<rosen::Biquad<Type>> biquad;
        
        // 2+ order
        //juce::OwnedArray<rosen::Biquad<Type>> setOfBiquads;
        rollOff order; // will determine the number of biquads to use in process
        Type coeffs[numCoeffs];
        void computeCoeffs();
        
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Filter)
    };
    
    template <typename Type>
    Filter<Type>::Filter()
    {
        biquad = std::make_unique<rosen::Biquad<Type>>(); // use default biquad
    }
    
    template <typename Type>
    Filter<Type>::Filter(FilterParameters<Type>& params)
    :parameters{params}
    {
        biquad = std::make_unique<rosen::Biquad<Type>>();
    }
    
    template <typename Type>
    Filter<Type>::~Filter()
    {

    }
    
    template <typename Type>
    Type Filter<Type>::process(const Type& sample) noexcept
    {
        Type outputSample = c0 * sample + d0 * biquad->process(sample);
        return outputSample;
        // do processng here
    }
    
    template <typename Type>
    void Filter<Type>::computeCoeffs()
    {
        std::fill(coeffs, coeffs + numCoeffs, 0);
        
    }
}
