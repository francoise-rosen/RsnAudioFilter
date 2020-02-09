/*
  ==============================================================================

    SineOscillator.h
    Created: 7 Feb 2020 11:53:58am
    Author:  SFD

  ==============================================================================
*/

#pragma once
#include "Phasor.h"

class SineOSC
{
public:
    SineOSC(Phasor* phase)
    :ph(phase)
    {
        std::cout << "SinOSC created\n";
    }
    
    forcedinline double getNextSample()
    {
        auto sample = std::sin(ph->getNextSample() * (double)MathConstants<float>::twoPi);
        return sample;
    }
    
private:
    Phasor* ph;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SineOSC)
};
