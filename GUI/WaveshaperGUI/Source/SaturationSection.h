/*
  ==============================================================================

    SaturationSection.h
    Created: 20 Sep 2020 12:28:26pm
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class SaturationSection  : public juce::Component
{
public:
    SaturationSection (juce::Colour parentBackground)
    {
        localBackground = parentBackground;
        addAndMakeVisible (&saturationSlider);
    }

    ~SaturationSection() override
    {
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (localBackground);
        
        /** draw vertical label */
        
        auto labelRect = getLocalBounds().removeFromLeft (getWidth() * 0.2f).removeFromBottom(getHeight() * 0.5f);
        g.setColour (juce::Colours::black);
        {
            juce::Graphics::ScopedSaveState state (g);
            g.addTransform (juce::AffineTransform::rotation (juce::MathConstants<float>::halfPi * 0.5f, getWidth() * 0.5f, getHeight() * 0.75f));
            g.drawFittedText ("SaTuRaTioN", labelRect, juce::Justification::centred, 1);
                            
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
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SaturationSection)
};
