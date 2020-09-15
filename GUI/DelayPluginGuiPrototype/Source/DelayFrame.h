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
        /* scale an object relative to its area */
        delayFrameLookAndFeel.setObjectScale(0.52f);
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
        feedbackSlider->setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
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
    
    void setFont(const juce::Font& font)
    {
        textFont = font;
    }
    
    void setTextColour(const juce::Colour& colour)
    {
        textColour = colour;
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll(windowColour);
        g.setColour(textColour);
        g.setFont(textFont);
        
        auto area = getLocalBounds().reduced(5.0f);
        g.drawFittedText("FeedBaCK", area.removeFromBottom(area.getHeight() * 0.25f), juce::Justification::centredTop, 1);
        
        
        
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
    juce::Colour textColour {juce::Colours::black};
    std::unique_ptr<juce::Slider> leftDelaySlider;
    std::unique_ptr<juce::Slider> rightDelaySlider;
    std::unique_ptr<juce::Slider> feedbackSlider;
    
    juce::Font textFont;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayFrame)
};
