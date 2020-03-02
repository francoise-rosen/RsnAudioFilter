/*
  ==============================================================================

    Oscillator.h
    Created: 27 Feb 2020 11:20:00am
    Author:  SFD

  ==============================================================================
*/

#pragma once
#include "SineCycle.h"

// set frequency and level of a single oscillator instance

class Oscillator : public Slider::Listener
{
public:
    Oscillator()
    :magnitude(0.0),
    frequency(0.0)
    {}
    
    Oscillator(Slider* magSlider, Slider* frqSlider)
    {
        // now we must create a SineCycle object
        magSlider->addListener(this);
        frqSlider->addListener(this);
        magnitude = magSlider->getValue();
        frequency = frqSlider->getValue();
        sineOSC.reset(new SineCycle());
    }
    
private:
    double magnitude;
    double frequency;
    std::unique_ptr<SineCycle> sineOSC;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Oscillator)
};
