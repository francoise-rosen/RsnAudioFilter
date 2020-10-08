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
        setLookAndFeel (&gainLookAndFeel);
        gainLookAndFeel.setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::blue.withBrightness(0.2f));
        gainLookAndFeel.setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colours::blue.withBrightness(0.2f));
        gainLookAndFeel.setColour (juce::Slider::thumbColourId, juce::Colours::blue.withBrightness(0.2f));
        addAndMakeVisible (&gainSlider);
        amountSlider.setNumDecimalPlacesToDisplay(2);
        addAndMakeVisible (&amountSlider);

    }

    virtual ~GainSection() override
    {
        setLookAndFeel (nullptr);
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
    WaveshaperLookAndFeel gainLookAndFeel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainSection)
};
