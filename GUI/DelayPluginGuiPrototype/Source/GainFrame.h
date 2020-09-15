/*
  ==============================================================================

    GainFrame.h
    Created: 14 Sep 2020 9:37:21am
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DelayLookAndFeel.h"

//==============================================================================
/*
*/
class GainFrame  : public juce::Component
{
public:
    GainFrame()
    {
        setLookAndFeel(&filterFrameLookAndFeel);
        gainSlider = std::make_unique<juce::Slider>(juce::Slider::SliderStyle::Rotary, juce::Slider::TextBoxBelow);
        addAndMakeVisible(gainSlider.get());
        amountSlider = std::make_unique<juce::Slider>(juce::Slider::SliderStyle::Rotary, juce::Slider::TextBoxBelow);
        addAndMakeVisible(amountSlider.get());
        typeBox = std::make_unique<juce::ComboBox>();
    }

    virtual ~GainFrame() override
    {
        setLookAndFeel(nullptr);
    }
    
    void setBackgroundColour(juce::Colour& background)
    {
        windowColour = background;
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::black.withAlpha(0.72f));
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(5.0f);
        typeBox->setBounds(area.removeFromTop(area.getHeight() * 0.3f));
        gainSlider->setBounds(area.removeFromRight(area.getWidth() * 0.45f));
        amountSlider->setBounds(area);

    }

private:
    juce::Colour windowColour;
    DelayLookAndFeel filterFrameLookAndFeel;
    std::unique_ptr<juce::Slider> gainSlider;
    std::unique_ptr<juce::Slider> amountSlider;
    std::unique_ptr<juce::ComboBox> typeBox;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainFrame)
};
