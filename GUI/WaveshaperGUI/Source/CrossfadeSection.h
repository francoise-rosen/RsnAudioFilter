/*
  ==============================================================================

    CrossfadeSection.h
    Created: 20 Sep 2020 12:28:47pm
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Arrow.h"

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
        crossfadeSlider.setLookAndFeel (&symmetricalLinearLookAndFeel);
        
        symmetricalRotaryLookAndFeel.setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::silver.brighter());
        symmetricalRotaryLookAndFeel.setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colours::white);
        symmetricalRotaryLookAndFeel.setColour (juce::Slider::thumbColourId, juce::Colours::silver.brighter());
        
        symmetricalLinearLookAndFeel.setColour (juce::Slider::trackColourId, juce::Colours::silver.darker());
        symmetricalLinearLookAndFeel.setThumbTriColour (juce::Colours::white);
        symmetricalLinearLookAndFeel.setColour (juce::Slider::backgroundColourId, juce::Colours::blue.withBrightness(0.25f));
        symmetricalLinearLookAndFeel.setLinearSliderThumbOuterRimColour (juce::Colours::black);
        symmetricalLinearLookAndFeel.setPointerType (SymmetricalLinearLookAndFeel::PointerType::Circle);
        
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
        
        /** add double arrow above the slider. */
        float arrowLength = 10.0f;
        float arrowY = ((sliderArea != nullptr) && (comboArea != nullptr)) ? sliderArea->getY() - (sliderArea->getY() - comboArea->getBottom()) * 0.25f : getHeight() * 0.75f;
        juce::Array<juce::Point<float>> arrowPointsRight
        {
            juce::Point<float> {
                getWidth() * 0.7f - arrowLength, arrowY
            },
            /** right endge */
            juce::Point<float> {
                getWidth() * 0.7f, arrowY
            }
        };
        juce::Array<juce::Point<float>> arrowPointsLeft
        {
            /** left edge */
            juce::Point<float> {
                getWidth() * 0.3f + arrowLength, arrowY
            },
            juce::Point<float> {
                getWidth() * 0.3f, arrowY
            }
        };
        Arrow<float> arrow {arrowPointsRight};
        arrow.setFill (true);
        arrow.draw (g, juce::Colours::orange, 2.0f, 8.0f, 10.0f);
        Arrow<float> arrowL {arrowPointsLeft};
        arrowL.setFill (true);
        arrowL.draw (g, juce::Colours::orange, 2.0f, 8.0f, 10.0f);
        
        juce::Path curve;
        curve.startNewSubPath (getWidth() * 0.3f + arrowLength, arrowY);
        curve.cubicTo ({getWidth() * 0.35f, arrowY}, {getWidth() * 0.4f, arrowY + 10.0f }, { getWidth() * 0.5f, arrowY + 2.0f});
        curve.cubicTo({ getWidth() * 0.55f, arrowY - 2.0f}, {getWidth() * 0.6f, arrowY - 10.0f }, { getWidth() * 0.7f - arrowLength, arrowY});
        g.strokePath (curve, juce::PathStrokeType {2.0f});

    }

    void resized() override
    {
        auto area = getLocalBounds().reduced (edge);
        symmetrySlider.setBounds (area.removeFromTop (area.getHeight() * 0.5f).reduced (edge));
        crossfadeSlider.setBounds (area.removeFromBottom (area.getHeight() * 0.5f));
        sliderArea = std::make_unique<juce::Rectangle<float>> (crossfadeSlider.getBounds().toFloat());
        auto buttonArea = area.removeFromTop (area.getHeight() * 0.5f);
        auto comboBoxArea = buttonArea;
        comboArea = std::make_unique<juce::Rectangle<float>> (comboBoxArea.toFloat());
//        functionA_toggle.setBounds (buttonArea.removeFromLeft (area.getWidth() * 0.25f).reduced(edge));
//        functionB_toggle.setBounds (buttonArea.removeFromRight (area.getWidth() * 0.25f).reduced(edge));
        functionA_box.setBounds (buttonArea.removeFromLeft (area.getWidth() * 0.5).reduced (edge, edge));
        functionB_box.setBounds (buttonArea.reduced (edge, edge));
        
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
    
    /** coordinates. */
    std::unique_ptr<juce::Rectangle<float>> sliderArea {nullptr};
    std::unique_ptr<juce::Rectangle<float>> comboArea {nullptr};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CrossfadeSection)
};
