/*
  ==============================================================================

    CrossfadeSection.h
    Created: 20 Sep 2020 12:28:47pm
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class CrossfadeSection  : public juce::Component
{
public:
    CrossfadeSection (juce::Colour parentBackground)
    {
        localBackground = parentBackground;
        symmetrySlider.setColour (juce::Slider::thumbColourId, parentBackground);
        symmetrySlider.setColour (juce::Slider::textBoxOutlineColourId, parentBackground.withAlpha(0.01f));
        crossfadeSlider.setColour (juce::Slider::thumbColourId, parentBackground);
        crossfadeSlider.setColour (juce::Slider::textBoxOutlineColourId, parentBackground.withAlpha(0.01f));
        
        functionA_toggle.setButtonText ("A");
        functionB_toggle.setButtonText ("B");
        
        addAndMakeVisible (&symmetrySlider);
        addAndMakeVisible (&crossfadeSlider);
        addAndMakeVisible (&functionA_box);
        addAndMakeVisible (&functionB_box);
        addAndMakeVisible (&functionA_toggle);
        addAndMakeVisible (&functionB_toggle);

    }

    virtual ~CrossfadeSection() override
    {
    }

    void paint (juce::Graphics& g) override
    {

        g.fillAll (localBackground);
        
        g.setColour (juce::Colours::black);
        auto area = getLocalBounds().reduced (edge);
        g.fillRoundedRectangle(area.toFloat(), edge * 2.0f);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced (edge);
        symmetrySlider.setBounds (area.removeFromTop (area.getHeight() * 0.5f).reduced (edge));
        crossfadeSlider.setBounds (area.removeFromBottom (area.getHeight() / 3.0f));
        auto buttonArea = area.removeFromBottom (area.getHeight() * 0.5f);
        functionA_toggle.setBounds (buttonArea.removeFromLeft (area.getWidth() * 0.25f).reduced(edge));
        functionB_toggle.setBounds (buttonArea.removeFromRight (area.getWidth() * 0.25f).reduced(edge));
        functionA_box.setBounds (area.removeFromLeft (area.getWidth() * 0.5).reduced (edge, edge + edge));
        functionB_box.setBounds (area.reduced (edge, edge + edge));
        
        

    }

private:
    const float edge {5.0f};
    juce::Colour localBackground;
    
    juce::Slider symmetrySlider {juce::Slider::SliderStyle::Rotary, juce::Slider::TextEntryBoxPosition::TextBoxBelow};
    juce::Slider crossfadeSlider {juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextEntryBoxPosition::TextBoxBelow};
    juce::ComboBox functionA_box;
    juce::ComboBox functionB_box;
    
    /** Both being off results in bypass saturation stage,
        which makes it to 2 stage filter + lfo unit.
     */
    juce::TextButton functionA_toggle;
    juce::TextButton functionB_toggle;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CrossfadeSection)
};
