/*
  ==============================================================================

    GainFrame.h
    Created: 14 Sep 2020 9:37:21am
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DelayLookAndFeel.h"

//==============================================================================
/*
*/
class GainFrame  : public juce::Component
{
public:
    GainFrame()
    {
        setLookAndFeel(&filterFrameLookAndFeel);
        gainSlider = std::make_unique<juce::Slider>(juce::Slider::SliderStyle::Rotary, juce::Slider::TextBoxBelow);
        gainSlider->setRange(-100.0, 24.0);
        gainSlider->setSkewFactorFromMidPoint(-6.0);
        gainSlider->setTextValueSuffix(" dB");
        gainSlider->setNumDecimalPlacesToDisplay(2);
        addAndMakeVisible(gainSlider.get());
        
        amountSlider = std::make_unique<juce::Slider>(juce::Slider::SliderStyle::Rotary, juce::Slider::TextBoxBelow);
        amountSlider->setRange(0.0, 100.0);
        amountSlider->setTextValueSuffix(" %");
        amountSlider->setNumDecimalPlacesToDisplay(2);
        addAndMakeVisible(amountSlider.get());
        
        comboLF.setColour(juce::ComboBox::backgroundColourId, windowColour.withAlpha(0.42f));
        comboLF.setColour(juce::ComboBox::outlineColourId, juce::Colours::white.withAlpha(0.72f));
        typeBox = std::make_unique<juce::ComboBox>();
        typeBox->setLookAndFeel(&comboLF);
        typeBox->addItemList(delayAlgorithm, 100);
        typeBox->setSelectedItemIndex(1);
        addAndMakeVisible(typeBox.get());
    }

    virtual ~GainFrame() override
    {
        setLookAndFeel(nullptr);
    }
    
    void setBackgroundColour (juce::Colour& background)
    {
        windowColour = background;
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (windowColour);
        g.setColour (juce::Colours::white.withAlpha(0.72f));
        auto area = getLocalBounds().reduced(5.0f);
        
        g.setFont(juce::Font("Monaco", "Plain", getHeight() * 0.08f));
        
        juce::Rectangle<int> typeLabelArea = area.withBottom (area.getHeight() * 0.2 + 5.0f).withRight (area.getWidth() * 0.4f);
        
        g.setColour(juce::Colours::black);
        g.fillRoundedRectangle(typeLabelArea.reduced (juce::jmin (typeLabelArea.getHeight() * 0.2f, typeLabelArea.getWidth() * 0.2f)).toFloat(), 5.0f);
        juce::Path typePointer;
        typePointer.startNewSubPath(typeLabelArea.getCentreX(), typeLabelArea.getCentreY());
        typePointer.lineTo(area.getWidth() * 0.4f + 5.0f, typeLabelArea.getCentreY());
        g.strokePath(typePointer, juce::PathStrokeType(21.0f));
        
        g.setColour (juce::Colours::white.withAlpha(0.72f));
        g.drawFittedText ("TyPe", area.withBottom (area.getHeight() * 0.23f).withRight (area.getWidth() * 0.4f), juce::Justification::centred, 1);
        g.drawFittedText ("aMOUNT", area.withTop (area.getHeight() * 0.26f).withRight (area.getWidth() * 0.55f), juce::Justification::topLeft, 1);
        g.drawFittedText("GaIN", area.withTop (area.getHeight() * 0.26f).withLeft (area.getWidth() * 0.55f), juce::Justification::topLeft, 1);
        
        
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(5.0f);
        auto comboArea = area.removeFromTop (area.getHeight() * 0.2f);
//        comboArea = comboArea.reduced (juce::jmin (5.0f, comboArea.getWidth() * 0.25f), comboArea.getHeight() * 0.2f);
        typeBox->setBounds (comboArea.removeFromRight (comboArea.getWidth() * 0.6f));
        gainSlider->setBounds (area.removeFromRight (area.getWidth() * 0.45f).reduced(area.getWidth() * 0.05f, area.getWidth() * 0.1f));
        amountSlider->setBounds (area);

    }

private:
    juce::Colour windowColour {juce::Colours::black.withAlpha(0.72f)};
    DelayLookAndFeel filterFrameLookAndFeel;
    ComboLookAndFeel comboLF;
    std::unique_ptr<juce::Slider> gainSlider;
    std::unique_ptr<juce::Slider> amountSlider;
    std::unique_ptr<juce::ComboBox> typeBox;
    
    juce::StringArray delayAlgorithm {"STeReO", "PiNG PoNG", "LCR"};
                         
    //juce::Rectangle<int> comboArea;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainFrame)
};
