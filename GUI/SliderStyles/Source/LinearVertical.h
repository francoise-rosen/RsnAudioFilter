/*
  ==============================================================================

    LinearVertical.h
    Created: 21 Oct 2020 4:57:54pm
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class LinearVertical  : public juce::Component
{
public:
    LinearVertical();
    ~LinearVertical() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider alphaVertical;
    juce::Slider alphaVerticalSym;
    juce::Slider alphaVerticalSymGradient;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LinearVertical)
};
