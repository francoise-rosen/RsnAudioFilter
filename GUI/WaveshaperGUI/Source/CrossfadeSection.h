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
        setLookAndFeel (&crossfadeLookAndFeel);
        localBackground = parentBackground;
        
        /** Crossfade. */
        crossfadeLookAndFeel.setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::silver.brighter());
        crossfadeLookAndFeel.setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colours::white);
        crossfadeLookAndFeel.setColour (juce::Slider::thumbColourId, juce::Colours::silver.brighter());
        crossfadeSlider.setColour (juce::Slider::thumbColourId, parentBackground);
        crossfadeSlider.setColour (juce::Slider::textBoxOutlineColourId, parentBackground.withAlpha (0.01f));
        
        functionA_toggle.setButtonText ("A");
        functionB_toggle.setButtonText ("B");
        
        /** Symmetrical Rotary Slider. */
        symmetrySlider.setRange (0.0, 1.0);
        symmetrySlider.setValue (0.5);
        symmetrySlider.setNumDecimalPlacesToDisplay (2);
        symmetrySlider.setColour (juce::Slider::thumbColourId, parentBackground);
        symmetrySlider.setColour (juce::Slider::textBoxOutlineColourId, parentBackground.withAlpha (0.01f));
        symmetrySlider.setLookAndFeel(&symmetricalRotaryLookAndFeel);
        symmetricalRotaryLookAndFeel.setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::silver.brighter());
        symmetricalRotaryLookAndFeel.setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colours::white);
        symmetricalRotaryLookAndFeel.setColour (juce::Slider::thumbColourId, juce::Colours::silver.brighter());
        crossfadeSlider.setLookAndFeel (&symmetricalLinearLookAndFeel);
        symmetricalLinearLookAndFeel.setColour (juce::Slider::trackColourId, juce::Colours::silver.darker());
        symmetricalLinearLookAndFeel.setThumbTriColour (juce::Colours::white);
        symmetricalLinearLookAndFeel.setColour (juce::Slider::backgroundColourId, juce::Colours::blue.withBrightness(0.25f));
        symmetricalLinearLookAndFeel.setLinearSliderThumbOuterRimColour (juce::Colours::black);
        
        addAndMakeVisible (&symmetrySlider);
        addAndMakeVisible (&crossfadeSlider);
        addAndMakeVisible (&functionA_box);
        addAndMakeVisible (&functionB_box);
        addAndMakeVisible (&functionA_toggle);
        addAndMakeVisible (&functionB_toggle);
        addAndMakeVisible (&negative);
        addAndMakeVisible (&positive);

    }

    virtual ~CrossfadeSection() override
    {
        symmetrySlider.setLookAndFeel (nullptr);
        crossfadeSlider.setLookAndFeel (nullptr);
        setLookAndFeel (nullptr);
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (localBackground);
        g.setColour (juce::Colours::black);
        auto area = getLocalBounds().reduced (edge);
        g.fillRoundedRectangle (area.toFloat(), edge * 2.0f);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced (edge);
        symmetrySlider.setBounds (area.removeFromTop (area.getHeight() * 0.5f).reduced (edge));
        crossfadeSlider.setBounds (area.removeFromBottom (area.getHeight() * 0.5f));
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
    
    WaveshaperLookAndFeel crossfadeLookAndFeel;
    SymmetricalRotaryLookAndFeel symmetricalRotaryLookAndFeel;
    SymmetricalLinearLookAndFeel symmetricalLinearLookAndFeel;
    
    juce::Label negative {"negative", "-"};
    juce::Label positive {"positive", "+"};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CrossfadeSection)
};
