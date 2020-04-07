/*
  ==============================================================================

    Sine.h
    Created: 5 Mar 2020 10:45:43am
    Author:  SFD

    Computes sine from phase ramp
  ==============================================================================
*/

#pragma once
#include "Phasor.h"


class SineOSC : public Signal
{
public:
    SineOSC(Phasor& ph)
    :phase(&ph),
    magnitude(0.0)
    {
        std::cout << "SinOSC created\n";
    }
    
    // this will initialise frequency, magnitude and phasor
    SineOSC(Phasor& ph, double mag)
    :phase(&ph),
    magnitude(mag)
    {
        jassert(magnitude >= 0.0 && magnitude <= 1.0);
    }
    
    virtual void updateFrequency(double newfreq) override
    {
        assert (phase != nullptr);
        phase->updateFrequency(newfreq);
    }
    
    forcedinline double getNextSample() noexcept override
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
    
    Phasor& accessPhasor() {return *phase;}
    
    virtual ~SineOSC() {};
    
private:
    // phasor is created somewhere
    Phasor* phase; // pointer to a phasor object
    double magnitude;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SineOSC)
};

