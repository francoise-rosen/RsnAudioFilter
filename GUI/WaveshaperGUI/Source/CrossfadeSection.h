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

class CrossfadeSection  : public juce::Component,
                          public juce::Slider::Listener

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
        
        /** Symmetrical Rotary Slider. */
        symmetrySlider.setRange (0.0, 1.0);
        symmetrySlider.setValue (0.5);
        symmetrySlider.addListener (this);
        symmetrySlider.setNumDecimalPlacesToDisplay (2);
        symmetrySlider.setColour (juce::Slider::thumbColourId, parentBackground);
        symmetrySlider.setColour (juce::Slider::textBoxOutlineColourId, parentBackground.withAlpha (0.01f));
        symmetrySlider.setLookAndFeel(&symmetricalRotaryLookAndFeel);
        crossfadeSlider.setLookAndFeel (&symmetricalLinearLookAndFeel);
        
        symmetricalRotaryLookAndFeel.setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::silver.brighter());
        symmetricalRotaryLookAndFeel.setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colours::white);
        symmetricalRotaryLookAndFeel.setColour (juce::Slider::thumbColourId, juce::Colours::silver.brighter());
        
        /** THESE DON'T WORK! WHY? */
        symmetrySlider.setColour (juce::Slider::textBoxBackgroundColourId, juce::Colours::black.withMultipliedAlpha (0.5f));
        symmetrySlider.setColour (juce::Slider::textBoxOutlineColourId, juce::Colours::black.withAlpha (0.0f));
        
        symmetricalLinearLookAndFeel.setColour (juce::Slider::trackColourId, juce::Colours::silver.darker());
        symmetricalLinearLookAndFeel.setThumbTriColour (juce::Colours::white);
        symmetricalLinearLookAndFeel.setColour (juce::Slider::backgroundColourId, juce::Colours::blue.withBrightness(0.25f));
        symmetricalLinearLookAndFeel.setLinearSliderThumbOuterRimColour (juce::Colours::black);
        symmetricalLinearLookAndFeel.setPointerType (SymmetricalLinearLookAndFeel::PointerType::Circle);
        
        negative.setLookAndFeel (&indicatorLabelLookAndFeel);
        positive.setLookAndFeel (&indicatorLabelLookAndFeel);
        negative.setText ("-", juce::NotificationType::dontSendNotification);
        positive.setText ("+", juce::NotificationType::dontSendNotification);
        negative.setJustificationType (juce::Justification::centred);
        positive.setJustificationType (juce::Justification::centred);
        negative.setColour (juce::Label::outlineColourId, juce::Colours::black);
        indicatorLabelLookAndFeel.setGradientOn (true);
        indicatorLabelLookAndFeel.setFillColour (localBackground.withAlpha (0.0f));
        //indicatorLabelLookAndFeel.setColour (juce::Label::outlineColourId, juce::Colours::black);
        
        // this is the fill colour, or the inner colour in gradient
        // both labels will have their fill colours set according
        // to the position of the symmerySlider
        // if gradient set the default outerColour == thisBackgroud and inner == labelBackground
        negative.setColour (juce::Label::backgroundColourId, juce::Colours::orange.withAlpha (0.5f));
        positive.setColour (juce::Label::backgroundColourId, juce::Colours::orange.withAlpha(0.5f));
        
        leftBox.addItemList(waveforms, 1);
        leftBox.setSelectedItemIndex(0);
        rightBox.addItemList(waveforms, 1);
        rightBox.setSelectedItemIndex(0);
        
        addAndMakeVisible (&symmetrySlider);
        addAndMakeVisible (&crossfadeSlider);
        addAndMakeVisible (&negative);
        addAndMakeVisible (&positive);
        addAndMakeVisible (&leftBox);
        addAndMakeVisible (&rightBox);

    }

    virtual ~CrossfadeSection() override
    {
        setLookAndFeel (nullptr);
        symmetrySlider.setLookAndFeel (nullptr);
        crossfadeSlider.setLookAndFeel (nullptr);
        negative.setLookAndFeel (nullptr);
        positive.setLookAndFeel (nullptr);
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (localBackground);
        g.setColour (juce::Colours::black);
        auto area = getLocalBounds().reduced (edge);
        g.fillRoundedRectangle (area.toFloat(), edge * 2.0f);
        
        /** Arrow and A B labels. */
        float arrowheadLength = 10.0f;
        /** Arrow y position. */
        float arrowY = ((sliderArea != nullptr) && (comboArea != nullptr)) ? sliderArea->getY() - (sliderArea->getY() - comboArea->getBottom()) * 0.25f : getHeight() * 0.75f;
        /** Arrow's x right and left. */
        float arrowXRight = getWidth() * 0.7f;
        juce::Array<juce::Point<float>> arrowPointsRight
        {
            juce::Point<float> {
                arrowXRight - arrowheadLength, arrowY
            },
            /** right endge */
            juce::Point<float> {
                arrowXRight, arrowY
            }
        };
        float arrowXLeft = getWidth() * 0.3f;
        juce::Array<juce::Point<float>> arrowPointsLeft
        {
            /** left edge */
            juce::Point<float> {
                arrowXLeft + arrowheadLength, arrowY
            },
            juce::Point<float> {
                arrowXLeft, arrowY
            }
        };
        Arrow<float> arrow {arrowPointsRight};
        arrow.setFill (true);
        arrow.draw (g, juce::Colours::orange, 2.0f, 8.0f, arrowheadLength);
        Arrow<float> arrowL {arrowPointsLeft};
        arrowL.setFill (true);
        arrowL.draw (g, juce::Colours::orange, 2.0f, 8.0f, arrowheadLength);
        
        juce::Path curve;
        curve.startNewSubPath (arrowXLeft + arrowheadLength, arrowY);
        curve.cubicTo ({ getWidth() * 0.35f, arrowY}, {getWidth() * 0.4f, arrowY + 10.0f }, { getWidth() * 0.5f, arrowY + 2.0f });
        curve.cubicTo({ getWidth() * 0.55f, arrowY - 2.0f}, { getWidth() * 0.6f, arrowY - 10.0f }, { arrowXRight - arrowheadLength, arrowY });
        g.strokePath (curve, juce::PathStrokeType {2.0f});
        
        /** Draw A and B labels. */
        g.setFont (defaultFont);
        g.drawFittedText ("A", juce::Rectangle<float> {arrowXLeft, arrowXLeft}.withCentre (juce::Point<float>{ arrowXLeft * 0.5f, arrowY }).toNearestInt(), juce::Justification::centred, 1);
        g.drawFittedText ("B", juce::Rectangle<float> {arrowXLeft, arrowXLeft}.withCentre (juce::Point<float>{ getWidth() - arrowXLeft * 0.5f, arrowY }).toNearestInt(), juce::Justification::centred, 1);
    
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced (edge);
        
        /** Upper half. */
        auto dialArea = area.removeFromTop (area.getHeight() * 0.5f);
        symmetrySlider.setBounds (dialArea.reduced (edge));
        auto labelDiameter = juce::jmin (dialArea.getHeight() * 0.15f, dialArea.getWidth() * 0.15f);
        negative.setBounds (juce::Rectangle<float> {labelDiameter, labelDiameter}.withCentre (juce::Point<float> { getWidth() * 0.25f, dialArea.getHeight() * 0.8f }).toNearestInt());
        positive.setBounds (juce::Rectangle<float> {labelDiameter, labelDiameter}.withCentre (juce::Point<float> { getWidth() * 0.75f, dialArea.getHeight() * 0.8f}).toNearestInt());
        
        crossfadeSlider.setBounds (area.removeFromBottom (area.getHeight() * 0.5f));
        sliderArea = std::make_unique<juce::Rectangle<float>> (crossfadeSlider.getBounds().toFloat());
        auto buttonArea = area.removeFromTop (area.getHeight() * 0.5f);
        comboArea = std::make_unique<juce::Rectangle<float>> (buttonArea.toFloat());
        leftBox.setBounds (buttonArea.removeFromLeft (area.getWidth() * 0.5).reduced (edge, edge));
        rightBox.setBounds (buttonArea.reduced (edge, edge));
        
    }
    
    void setFont (juce::Font f)
    {
        defaultFont = f;
    }
    
    void sliderValueChanged (juce::Slider* slider) override
    {
        if (slider == &symmetrySlider)
        {
            float val = static_cast<float> (symmetrySlider.getValue());
            negative.setColour (juce::Label::backgroundColourId, juce::Colours::orange.withAlpha (1.0f - val));
            positive.setColour (juce::Label::backgroundColourId, juce::Colours::orange.withAlpha (val));
        }
    }

private:
    const float edge {5.0f};
    juce::Colour localBackground;
    
    juce::Slider symmetrySlider {juce::Slider::SliderStyle::Rotary, juce::Slider::TextEntryBoxPosition::TextBoxBelow};
    juce::Slider crossfadeSlider {juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextEntryBoxPosition::TextBoxBelow};
    
    WaveshaperLookAndFeel crossfadeLookAndFeel;
    SymmetricalRotaryLookAndFeel symmetricalRotaryLookAndFeel;
    SymmetricalLinearLookAndFeel symmetricalLinearLookAndFeel;
    IndicatorLookAndFeel indicatorLabelLookAndFeel;
    
    juce::Label negative {"negative", "-"};
    juce::Label positive {"positive", "+"};
    juce::Font defaultFont {"Monaco", "Plain", 14.0f};
    const juce::StringArray waveforms {"SINe", "CoS", "TaNH"};
    
    /** coordinates. */
    std::unique_ptr<juce::Rectangle<float>> sliderArea {nullptr};
    std::unique_ptr<juce::Rectangle<float>> comboArea {nullptr};
    
    juce::ComboBox leftBox;
    juce::ComboBox rightBox;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CrossfadeSection)
};


