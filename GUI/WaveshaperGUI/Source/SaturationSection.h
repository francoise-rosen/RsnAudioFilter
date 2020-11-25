/*
  ==============================================================================

    SaturationSection.h
    Created: 20 Sep 2020 12:28:26pm
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "WaveshaperLookAndFeel.h"

//==============================================================================
/*
*/
class SaturationSection  : public juce::Component
{
public:
    SaturationSection (juce::Colour parentBackground)
    {
        setLookAndFeel (&customLookAndFeel);
        customLookAndFeel.setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
        customLookAndFeel.setColour (juce::Slider::thumbColourId, juce::Colours::orange.darker());
        localBackground = parentBackground;
        saturationSlider.setLookAndFeel (&rotaryLookAndFeel);
        saturationSlider.setColour (juce::Slider::backgroundColourId, juce::Colours::black);
        saturationSlider.setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::black);
        addAndMakeVisible (&saturationSlider);
    }

    virtual ~SaturationSection() override
    {
        setLookAndFeel (nullptr);
        saturationSlider.setLookAndFeel (nullptr);
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (localBackground);
        
        /** draw vertical label */
        
//        auto labelRect = getLocalBounds().removeFromLeft (getWidth() * 0.5f).removeFromBottom(getHeight() * 0.5f);
        auto area = getLocalBounds().reduced (edge);
        auto labelRect = getLocalBounds().removeFromBottom (getHeight() * 0.25f).withLeft (getWidth() * 0.25f).withRight (getWidth() * 0.75f);
        g.setColour (juce::Colours::black);
        
        {
            juce::Graphics::ScopedSaveState state (g);
            g.addTransform (juce::AffineTransform::rotation (juce::MathConstants<float>::halfPi * 0.5f, getWidth() * 0.5f, getHeight() * 0.5f));
            g.drawRect (labelRect);
            g.drawFittedText ("SaTuRaTioN", labelRect, juce::Justification::centred, 1);
            g.setColour (juce::Colours::red);
            g.drawRect (area);
                            
        }
    }

    void resized() override
    {
        auto area = getLocalBounds();
        saturationSlider.setBounds (area.withLeft (area.getWidth() * 0.25f).reduced (edge));

    }

private:
    const float edge {5.0f};
    juce::Slider saturationSlider {juce::Slider::SliderStyle::Rotary, juce::Slider::TextEntryBoxPosition::TextBoxBelow};
    juce::Colour localBackground;
    WaveshaperLookAndFeel customLookAndFeel;
    RotarySliderBigLookAndFeel rotaryLookAndFeel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SaturationSection)
};
