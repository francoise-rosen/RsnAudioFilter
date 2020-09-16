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

        filterFrameLookAndFeel.setObjectScale(0.5f);
        setLookAndFeel(&filterFrameLookAndFeel);
        
        filterFrameLookAndFeel.setObjectColour(juce::Colours::white.withAlpha(0.86f));
        filterFrameLookAndFeel.setRimColour(juce::Colours::black);
        frequencySlider = std::make_unique<juce::Slider>(juce::Slider::SliderStyle::Rotary, juce::Slider::TextBoxBelow);
        filterTypeSlider = std::make_unique<juce::Slider>(juce::Slider::SliderStyle::Rotary, juce::Slider::TextBoxBelow);
        qSlider = std::make_unique<juce::Slider>(juce::Slider::SliderStyle::Rotary, juce::Slider::TextBoxBelow);
        frequencySlider->setRange(20.0, 20000);
        frequencySlider->setTextValueSuffix(" Hz");
        frequencySlider->setNumDecimalPlacesToDisplay(2);
        addAndMakeVisible(qSlider.get());
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
        g.fillAll(windowColour.withBrightness(0.72f));
        g.setFont(localFont);
        juce::Colour textColour = juce::Colours::white.withAlpha(0.86f);
        juce::Colour labelColour = juce::Colours::black.withAlpha(0.77f);
        
        auto area = getLocalBounds();
        g.setColour(juce::Colours::orange.withBrightness(0.77f));

        /* edge */
        juce::Path p;
        p.startNewSubPath(area.getX(), area.getY() + getHeight() * 0.27f);
        p.lineTo(area.getX(), area.getBottom());
        p.lineTo(area.getRight(), area.getBottom());
        g.strokePath(p, juce::PathStrokeType(3.0f));
        
        /* section label */
        g.setColour(labelColour);
        auto labelSection = area.removeFromTop(getHeight() * labelHeightRatio).reduced(getWidth() * 0.24f, edge);
        g.fillRect(labelSection);
        g.setColour(textColour);
        g.drawFittedText("VSF", labelSection, juce::Justification::centred, 1);
        
        /* knob labels */
        g.setColour(labelColour);
        auto freqArea = area.removeFromTop(area.getHeight() * 0.4f);
        auto freqLabelRect = freqArea.withBottom(freqArea.getY() + getHeight() * labelHeightRatio).withLeft(getWidth() * 0.67f).reduced(edge);
        auto qArea = area.removeFromTop(area.getHeight() * 0.5f);
        auto qLabelRect = qArea.withBottom(qArea.getY() + getHeight() * labelHeightRatio).withLeft(getWidth() * 0.67f).reduced(edge);
        auto filterLabelRect = area.withBottom(area.getY() + getHeight() * labelHeightRatio).withLeft(getWidth() * 0.67f).reduced(edge);
        g.fillRoundedRectangle(freqLabelRect.toFloat(), 3.0f);
        g.fillRoundedRectangle(qLabelRect.toFloat(), 3.0f);
        g.fillRoundedRectangle(filterLabelRect.toFloat(), 3.0f);
        
        /* label lines */
        
        juce::Path freqLabelLine;
        freqLabelLine.startNewSubPath(freqLabelRect.getCentreX(), freqLabelRect.getBottom());
        freqLabelLine.lineTo(freqLabelRect.getCentreX(), freqArea.getCentreY());
        freqLabelLine.lineTo(getWidth() * 0.5f, freqArea.getCentreY());
        g.strokePath(freqLabelLine, juce::PathStrokeType(3.0f));
        
        juce::Path filterLabelLine;
        filterLabelLine.startNewSubPath(filterLabelRect.getCentreX(), filterLabelRect.getBottom());
        filterLabelLine.lineTo(filterLabelRect.getCentreX(), area.getCentreY());
        filterLabelLine.lineTo(getWidth() * 0.5f, area.getCentreY());
        g.strokePath(filterLabelLine, juce::PathStrokeType(3.0f));
        
        juce::Path qLabelLine;
        qLabelLine.startNewSubPath(qLabelRect.getCentreX(), qLabelRect.getBottom());
        qLabelLine.lineTo(qLabelRect.getCentreX(), qArea.getCentreY());
        qLabelLine.lineTo(getWidth() * 0.5f, qArea.getCentreY());
        g.strokePath(qLabelLine, juce::PathStrokeType(3.0f));
        
        /* set label text */
        g.setFont(juce::Font("Monaco", "Plane", freqLabelRect.getHeight() * 0.57f));
        g.setColour(textColour);
        g.drawFittedText("FReQ", freqLabelRect, juce::Justification::centred, 1);
        g.drawFittedText("Kju", qLabelRect, juce::Justification::centred, 1);
        g.drawFittedText("FiLTeR", filterLabelRect, juce::Justification::centred, 1);
        
        /* lines to labels */
        g.setColour(juce::Colours::black.withAlpha(1.0f));
        g.drawLine(freqArea.getCentreX(), freqArea.getCentreY(), freqArea.getCentreX(), getBottom() - 15.0f, 2.0f);
        
    }

    void resized() override
    {
        auto area = getLocalBounds().removeFromBottom(getHeight() * (1.0f - labelHeightRatio));
        frequencySlider->setBounds(area.removeFromTop(area.getHeight() * 0.4f));
        qSlider->setBounds(area.removeFromTop(area.getHeight() * 0.5f));
        filterTypeSlider->setBounds(area);
        repaint();
        
    }

private:
    DelayLookAndFeel filterFrameLookAndFeel;
    juce::Colour windowColour;
    juce::Font localFont;
    float labelHeightRatio {0.075f};
    float edge {3.0f};
    std::unique_ptr<juce::Slider> frequencySlider;
    std::unique_ptr<juce::Slider> filterTypeSlider;
    std::unique_ptr<juce::Slider> qSlider;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterFrame)
};
