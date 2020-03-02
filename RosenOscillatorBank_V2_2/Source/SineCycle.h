/*
  ==============================================================================

    SineOscillator.h
    Created: 7 Feb 2020 11:53:58am
    Author:  SFD

  ==============================================================================
*/

#pragma once
#include "Phasor.h"

// SineCycle uses external Phasor or internal Phasor
// in first example it does not own a Phasor object
// in the second example it does own a Phasor object
class SineCycle
{
public:
    SineCycle(Phasor* ph)
    :phase(ph),
    magnitude(1.0)
    {
        std::cout << "SinOSC created\n";
    }
    
    // this will initialise frequency, magnitude and phasor
    SineCycle(Phasor* ph, double mag)
    :phase(ph),
    magnitude(mag)
    {
        jassert(magnitude >= 0.0 && magnitude <= 1.0);
    }
    
    // using internal Phasor
    SineCycle(double f, bool resetPhase = false, double startPhase = 0.0)
    :magnitude(1)
    {
        // I need to create an object of Phasor here
    }
    
    
    forcedinline double getNextSample()
    {
        auto sample = std::sin(phase->getNextSample() * (double)MathConstants<float>::twoPi);
        sample *= magnitude;
        return sample;
    }
    
    void setMagnitude(double mag)
    {
        //if (mag <= 0.0 || mag >= 1.0) return; // don't change magnitude if the entry is invalid
        magnitude = mag;
    };
    
    void setFrequency(double frq)
    {
        phase->updateFrequency(frq);
    }
    
    // ?
    Phasor& accessPhasor() const {return *phase;}
    
private:
    Phasor* phase;
    double magnitude;
    //double frequency;
    // phase reset

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SineCycle)
};
