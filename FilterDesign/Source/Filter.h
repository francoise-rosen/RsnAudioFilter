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
        ~Filter();
        
        // this uses additional c and d coeffs (c - dry, d - wet multiplier)
        enum coeffArray {a0, a1, a2, b1, b2, c, d, NumOfCoeff};
        
        Type process(const Type& sample);
    private:
        FilterParameters<Type> params;
        
        // 1-2 order
        std::unique_ptr<rosen::Biquad<Type>> biquad;
        
        // 2+ order
        juce::OwnedArray<rosen::Biquad<Type>> setOfBiquads;
        rollOff order; // will determine the number of biquads to use in process
        void resetAll(); // reset all biquads and state
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Filter)
    };
    
    template <typename Type>
    Filter<Type>::Filter()
    {
        biquad = std::make_unique<rosen::Biquad<Type>>(); // use default biquad
    }
    
    template <typename Type>
    Filter<Type>::~Filter()
    {

    }
    
    template <typename Type>
    Type Filter<Type>::process(const Type& sample)
    {
        Type outputSample = sample;
        return outputSample;
        // do processng here
    }
}
