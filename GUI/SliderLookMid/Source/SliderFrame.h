/*
  ==============================================================================

    SliderFrame.h
    Created: 12 Sep 2020 9:47:22am
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SliderMidLookAndFeel.h"

//==============================================================================
/*
*/
class SliderFrame  : public juce::Component
{
public:
    SliderFrame()
    {
        //customSliderMid.isWithGradient(false);
        setLookAndFeel(&customSliderMid);
        addAndMakeVisible(apple);
        addAndMakeVisible(orange);

    }

    ~SliderFrame() override
    {
        DBG("Slider Frame distructed\n");
        setLookAndFeel(nullptr);
    
    }

    void paint (juce::Graphics& g) override
    {
        juce::Colour background = juce::Colours::lightcyan.withBrightness(0.75f).withAlpha(0.85f);
        g.fillAll(background);
        customSliderMid.setBackgroundColour(background);
    }

    void resized() override
    {
        auto sliderArea = getLocalBounds().reduced(edge);
        
        apple.setBounds(sliderArea.removeFromLeft(sliderArea.getWidth()/3));
        orange.setBounds(sliderArea);
    }

private:
    juce::Slider apple {juce::Slider::SliderStyle::Rotary, juce::Slider::TextBoxBelow };
    juce::Slider orange {juce::Slider::SliderStyle::Rotary, juce::Slider::TextBoxBelow };
    SliderMidLookAndFeel customSliderMid;
    //SliderMidLookAndFeel customSlider2;
    const float edge {3.0f};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderFrame)
};
