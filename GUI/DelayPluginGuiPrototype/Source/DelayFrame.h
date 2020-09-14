/*
  ==============================================================================

    DelayFrame.h
    Created: 14 Sep 2020 9:36:48am
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DelayLookAndFeel.h"

//==============================================================================
/*
*/
class DelayFrame  : public juce::Component
{
public:
    DelayFrame()
    {
        setLookAndFeel(&delayFrameLookAndFeel);
        leftDelaySlider = std::make_unique<juce::Slider>(juce::Slider::SliderStyle::Rotary, juce::Slider::TextBoxBelow);
        rightDelaySlider = std::make_unique<juce::Slider>(juce::Slider::SliderStyle::Rotary, juce::Slider::TextBoxBelow);
        feedbackSlider = std::make_unique<juce::Slider>(juce::Slider::SliderStyle::Rotary, juce::Slider::TextBoxBelow);
        addAndMakeVisible(leftDelaySlider.get());
        addAndMakeVisible(rightDelaySlider.get());
        addAndMakeVisible(feedbackSlider.get());
        feedbackKnobLook.setBackgroundColour(juce::Colours::black);
        feedbackKnobLook.setObjectColour(juce::Colours::darkgreen.withBrightness(0.2f));
        feedbackKnobLook.setRimColour(juce::Colours::black);
        feedbackSlider->setLookAndFeel(&feedbackKnobLook);
        

    }

    virtual ~DelayFrame() override
    {
        setLookAndFeel(nullptr);
    }
    
    void setBackgroundColour(juce::Colour& background)
    {
        windowColour = background;
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll(windowColour);
        
        

    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(5.0f);
        auto feedbackKnobArea = area.removeFromBottom(getHeight() * 0.25f);
        
        feedbackSlider->setBounds(feedbackKnobArea);
        leftDelaySlider->setBounds(area.removeFromTop(area.getHeight() * 0.5f).removeFromLeft(getWidth() * 0.8f));
        rightDelaySlider->setBounds(area.removeFromLeft(getWidth() * 0.8f));

    }

private:
    DelayLookAndFeel delayFrameLookAndFeel;
    DelayLookAndFeel feedbackKnobLook;
    juce::Colour windowColour {juce::Colours::white};
    std::unique_ptr<juce::Slider> leftDelaySlider;
    std::unique_ptr<juce::Slider> rightDelaySlider;
    std::unique_ptr<juce::Slider> feedbackSlider;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayFrame)
};
