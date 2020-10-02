/*
  ==============================================================================

    GainSection.h
    Created: 20 Sep 2020 12:28:58pm
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class GainSection  : public juce::Component
{
public:
    GainSection(juce::Colour& parentBackground)
    :localBackground {parentBackground}
    {
        addAndMakeVisible (&gainSlider);
        amountSlider.setNumDecimalPlacesToDisplay(2);
        addAndMakeVisible (&amountSlider);

    }

    ~GainSection() override
    {
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (localBackground);
    }

    void resized() override
    {
        auto amountSliderArea = getLocalBounds().removeFromTop (getHeight() * 0.55f).removeFromLeft (getWidth() * 0.55f);
        amountSlider.setBounds (amountSliderArea.reduced (edge));
        auto gainSliderArea = getLocalBounds().removeFromBottom (getHeight() * 0.75f).removeFromRight (getWidth() * 0.75f);
        gainSlider.setBounds (gainSliderArea.reduced (edge));
       
    }

private:
    const float edge {5.0f};
    juce::Colour localBackground;
    juce::Slider gainSlider {juce::Slider::SliderStyle::Rotary, juce::Slider::TextEntryBoxPosition::NoTextBox};
    juce::Slider amountSlider {juce::Slider::SliderStyle::Rotary, juce::Slider::TextEntryBoxPosition::NoTextBox};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainSection)
};
