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
// up to 6 order
// for instance 3rd order ButterLPF with have 1 2nd Order ButterLPF Biquad followed
// by 1st order ButterLPF Biquad

// this version has no peak filter!

// for delay plugin use only butter and lo / hi shelves


namespace sfd{
    enum class filterAlgorithm {
        LPF, HPF, BPF, BPS, ButterLPF, ButterHPF, ButterBPF, ButterBPS, LoShelf, HiShelf
    };
    
    enum coeffArray {a0, a1, a2, b1, b2, c, d, numOfCoeff};
    enum rollOff {db6=1, db12, db18, db24, db30, db36, db44};
    
    struct FilterParameters
    {
        FilterParameters() {}
        FilterParameters& operator=(const FilterParameters& params);
        
        filterAlgorithm algorithm = filterAlgorithm::LPF;
        double freq = 100.0;
        double Q = 0.707;
        int order = 1;
    };
    
    template <typename T>
    class Filter
    {
    public:
        Filter();
        ~Filter();
        
        T process(const T& sample);
    private:
        FilterParameters params;
        rollOff order; // will determine the number of biquads to use in process
    };
}
