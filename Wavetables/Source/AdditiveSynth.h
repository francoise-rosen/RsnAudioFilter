/*
  ==============================================================================

    AdditiveSynth.h
    Created: 23 Jan 2020 9:58:06am
    Author:  SFD

  ==============================================================================
*/

#pragma once
#include "Phasor.h"
#include "Sine.h"

class AdditiveSynth
{
    public:
    
    AdditiveSynth()
    :numOfOscillators(4)
    {
        // inialise
    }
    
    
    
    
    AdditiveSynth(const int& numOfPartials)
    :numOfOscillators{numOfPartials}
    {
    }
    
    
    private:
    int numOfOscillators;
    Sine* sineOsc; // like enum in vectors;
    Phasor* currentPhase; // one phase for all, so everything runs in phase
    // but use different different rates of it
}
