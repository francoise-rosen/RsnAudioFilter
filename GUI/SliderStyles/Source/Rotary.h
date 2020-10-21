/*
  ==============================================================================

    Rotary.h
    Created: 21 Oct 2020 5:03:51pm
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class Rotary  : public juce::Component
{
public:
    Rotary();
    ~Rotary() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider alphaRotary;
    juce::Slider alphaSymRotary; /** Track on / off. */
    juce::Slider alphaSymRotaryGradientTrack;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Rotary)
};
