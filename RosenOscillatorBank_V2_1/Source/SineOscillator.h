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
    :ph(phase),
    magnitude(0.0)
    {
        std::cout << "SinOSC created\n";
    }
    
    // this will initialise frequency, magnitude and phasor
    SineOSC(Phasor* phase, double mag)
    :ph(phase),
    magnitude(mag)
    {
        jassert(magnitude >= 0.0 && magnitude <= 1.0);
    }
    
    forcedinline double getNextSample()
    {
        auto sample = std::sin(ph->getNextSample() * (double)MathConstants<float>::twoPi);
        sample *= magnitude;
        return sample;
    }
    
    void setMagnitude(double mag)
    {
        //if (mag <= 0.0 || mag >= 1.0) return; // don't change magnitude if the entry is invalid
        magnitude = mag;
    };
    
    Phasor& accessPhasor() {return *ph;}
    
private:
    Phasor* ph;
    double magnitude;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SineOSC)
};
