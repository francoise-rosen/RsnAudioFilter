/*
  ==============================================================================

    FilterFrame.h
    Created: 14 Sep 2020 9:37:08am
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DelayLookAndFeel.h"

//==============================================================================
/*
*/
class FilterFrame  : public juce::Component
{
public:
    FilterFrame()
    {
        setLookAndFeel(&filterFrameLookAndFeel);
        filterFrameLookAndFeel.setObjectColour(juce::Colours::white.withAlpha(0.86f));
        filterFrameLookAndFeel.setRimColour(juce::Colours::black);
        frequencySlider = std::make_unique<juce::Slider>(juce::Slider::SliderStyle::Rotary, juce::Slider::TextBoxBelow);
        filterTypeSlider = std::make_unique<juce::Slider>(juce::Slider::SliderStyle::Rotary, juce::Slider::TextBoxBelow);
        addAndMakeVisible(frequencySlider.get());
        addAndMakeVisible(filterTypeSlider.get());
    }
   
    virtual ~FilterFrame() override
    {
        setLookAndFeel(nullptr);
    }
    
    void setBackgroundColour(juce::Colour& background)
    {
        windowColour = background;
    }
    
    void setFont(const juce::Font& f)
    {
        localFont = f;
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::white.withAlpha(0.72f));
        g.setFont(localFont);
        
        auto area = getLocalBounds();
        g.setColour(juce::Colours::orange.withBrightness(0.77f));

        /* edge */
        juce::Path p;
        p.startNewSubPath(area.getX(), area.getY());
        p.lineTo(area.getX(), area.getBottom());
        p.lineTo(area.getRight(), area.getBottom());
        g.strokePath(p, juce::PathStrokeType(3.0f));
        
        /* section label */
        g.setColour(juce::Colours::black.withAlpha(0.77f));
        auto labelSection = area.removeFromTop(getHeight() * labelHeightRatio).reduced(getWidth() * 0.24f, getHeight() * 0.01f);
        g.fillRect(labelSection);
        g.setColour(juce::Colours::white.withAlpha(0.86f));
        g.drawFittedText("VSF", labelSection, juce::Justification::centred, 1);
        
        
    }

    void resized() override
    {
        auto area = getLocalBounds().removeFromBottom(getHeight() * (1.0f - labelHeightRatio));
        frequencySlider->setBounds(area.removeFromTop(area.getHeight() * 0.5f));
        filterTypeSlider->setBounds(area);
        
    }

private:
    DelayLookAndFeel filterFrameLookAndFeel;
    juce::Colour windowColour;
    juce::Font localFont;
    float labelHeightRatio {0.1f};
    std::unique_ptr<juce::Slider> frequencySlider;
    std::unique_ptr<juce::Slider> filterTypeSlider;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterFrame)
};
