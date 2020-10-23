/*
  ==============================================================================

    LinearVertical.h
    Created: 21 Oct 2020 4:57:54pm
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "AlphaOneLookAndFeel.h"

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
    juce::Slider alphaVertical {juce::Slider::LinearVertical, juce::Slider::TextEntryBoxPosition::TextBoxBelow};
    juce::Slider alphaVerticalSym {juce::Slider::LinearVertical, juce::Slider::TextEntryBoxPosition::TextBoxBelow};
    juce::Slider alphaVerticalSymGradient {juce::Slider::LinearVertical, juce::Slider::TextEntryBoxPosition::TextBoxBelow};
    
    AlphaOneLookAndFeel alphaOneLookAndFeel;
    AlphaOneSymmetricalSlider alphaOneSymLF;
    AlphaOneSymmetricalSlider_V2 alphaOneSymGradientLF;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LinearVertical)
};
