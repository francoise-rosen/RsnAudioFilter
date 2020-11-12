/*
  ==============================================================================

    LFOSection.h
    Created: 20 Sep 2020 12:27:44pm
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "WaveshaperLookAndFeel.h"

//==============================================================================
/*
*/
class LFOSection  : public juce::Component
{
public:
    LFOSection();
    virtual ~LFOSection() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider lfoRateSlider { juce::Slider::SliderStyle::Rotary, juce::Slider::TextEntryBoxPosition::TextBoxBelow };
    juce::Label label_A;
    WaveshaperLookAndFeel waveshaperLookAndFeel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOSection)
};
