/*
  ==============================================================================

    Sine.h
    Created: 23 Jan 2020 9:49:32am
    Author:  SFD
    
    Sine - direct computation

  ==============================================================================
*/

#pragma once
#include "Phasor.h"


class Sine
{
public:
    Sine()
    :phase(new Phasor())
    {}
    
    // set frequency directly to sine?
    Sine(double frequency, double sampleRate)
    :phase(new Phasor(frequency, sampleRate))
    {
        
    }
    
    // or create a Phasor object elsewhere?
    Sine(Phasor* p)
    :phase(p)
    {
        
    }
    // use this only if we're creating new Phasor!
    ~Sine() { delete phase;}
    
    
private:
    Phasor* phase;
    double localSampleRate;
};
