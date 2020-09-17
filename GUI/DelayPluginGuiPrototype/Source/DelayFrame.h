/*
  ==============================================================================

    DelayFrame.h
    Created: 14 Sep 2020 9:36:48am
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DelayLookAndFeel.h"

//==============================================================================
/*
*/
class DelayFrame  : public juce::Component
{
public:
    DelayFrame()
    {
        /* scale an object relative to its area */
        delayFrameLookAndFeel.setObjectScale(0.52f);
        setLookAndFeel(&delayFrameLookAndFeel);
        leftDelaySlider = std::make_unique<juce::Slider>(juce::Slider::SliderStyle::Rotary, juce::Slider::TextBoxBelow);
        rightDelaySlider = std::make_unique<juce::Slider>(juce::Slider::SliderStyle::Rotary, juce::Slider::TextBoxBelow);
        feedbackSlider = std::make_unique<juce::Slider>(juce::Slider::SliderStyle::Rotary, juce::Slider::TextBoxBelow);
        leftDelaySlider->setRange(0.01, 2000.0);
        rightDelaySlider->setRange(0.01, 2000.0);
        leftDelaySlider->setTextValueSuffix(" ms");
        rightDelaySlider->setTextValueSuffix(" ms");
        leftDelaySlider->setNumDecimalPlacesToDisplay(2);
        rightDelaySlider->setNumDecimalPlacesToDisplay(2);
        addAndMakeVisible(leftDelaySlider.get());
        addAndMakeVisible(rightDelaySlider.get());
        addAndMakeVisible(feedbackSlider.get());
        feedbackKnobLook.setBackgroundColour(juce::Colours::black);
        feedbackKnobLook.setObjectColour(juce::Colours::darkgreen.withBrightness(0.2f));
        feedbackKnobLook.setRimColour(juce::Colours::black);
        feedbackSlider->setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        feedbackSlider->setLookAndFeel(&feedbackKnobLook);

    }

    virtual ~DelayFrame() override
    {
        setLookAndFeel(nullptr);
    }
    
    void setBackgroundColour (juce::Colour& background)
    {
        windowColour = background;
    }
    
    void setFont (const juce::Font& font)
    {
        textFont = font;
    }
    
    void setTextColour (const juce::Colour& colour)
    {
        textColour = colour;
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll(windowColour);
        g.setColour(textColour);
        g.setFont(textFont);
        
        /* LABELS */
        auto delayLabel = getLocalBounds().removeFromTop(getHeight() * 0.051f).removeFromLeft(getWidth() * 0.42f).reduced(edge);
        g.fillRect(delayLabel);
        
        g.setColour(windowColour.withAlpha(1.0f));
        g.drawFittedText("DeLay", delayLabel, juce::Justification::centred, 1);
        g.setColour(textColour);
        
        auto thisArea = getLocalBounds();
        auto lowerArea = thisArea.removeFromBottom(getHeight() * 0.25f);
        auto upperSection = thisArea.removeFromTop(thisArea.getHeight() * 0.5f);
        auto midSection = std::move(thisArea);
        g.drawFittedText("FeedBk", lowerArea.withRight(getWidth() * 0.9f).reduced(edge, 0), juce::Justification::topRight, 1);
        g.drawFittedText("LeFT", upperSection.withRight(getWidth() * 0.9f).withBottom(upperSection.getHeight() * 0.5f).reduced(edge), juce::Justification::centredRight, 1);
        g.drawFittedText("RiGHT", midSection.withRight(getWidth() * 0.9f).withBottom(upperSection.getHeight() + midSection.getHeight() * 0.5f - 10.0f).reduced(edge), juce::Justification::centredRight, 1);
        
        /* SIGNAL FLOW ARROWS */
        
        /* to the right delay slider */
        g.setColour (textColour);
        juce::Path fbFlowR;
        fbFlowR.startNewSubPath (getWidth() * 0.25f, getHeight() * 0.9f);
        fbFlowR.lineTo (getWidth() * 0.05f, getHeight() * 0.9f);
        fbFlowR.lineTo (getWidth() * 0.05f, getHeight() * 0.65f);
        fbFlowR.lineTo (getWidth() * 0.15f, getHeight() * 0.6f);
        fbFlowR = fbFlowR.createPathWithRoundedCorners(14.0f);
        g.strokePath (fbFlowR, juce::PathStrokeType(2.0f));
        
        fbFlowR.addArrow (juce::Line<float>{getWidth() * 0.15f, getHeight() * 0.6f, getWidth() * 0.15f, getHeight() * 0.6f}, 0.5f, 3.0f, 15.0f);
        
        /* to the left delay slider */
        juce::Path fbFlowL;
        fbFlowL.startNewSubPath (getWidth() * 0.05f, getHeight() * 0.7f);
        fbFlowL.lineTo (getWidth() * 0.05f, getHeight() * 0.29f);
        fbFlowL.lineTo (getWidth() * 0.15f, getHeight() * 0.24f);
        fbFlowL = fbFlowL.createPathWithRoundedCorners(14.0f);
        g.strokePath (fbFlowL, juce::PathStrokeType(2.0f));
        
        /* left delay out */
        
        juce::Path leftDelayOutPath;
        float xStart = getWidth() * 0.75f;
        float xVertical = getWidth() * 0.9f;
        float yCentreL = getHeight() * 0.1875f;
        float yCentreFb = getHeight() - getHeight() * 0.125f;
        float farX = getWidth() * 0.96f;
        leftDelayOutPath.startNewSubPath(xStart, yCentreL);
        leftDelayOutPath.lineTo(xVertical, yCentreL);
        leftDelayOutPath.lineTo(xVertical, yCentreFb);
        leftDelayOutPath.lineTo(farX, yCentreFb);
        leftDelayOutPath = leftDelayOutPath.createPathWithRoundedCorners(14.0f);
        g.strokePath(leftDelayOutPath, juce::PathStrokeType(2.0f));
        
        /* right delay out and to the VSF */
        
        juce::Path rightDelayOutPath;
        float yCentreR = getHeight() * 0.5625f;
        float yRight = yCentreR + getHeight() * 0.09f;
        rightDelayOutPath.startNewSubPath(xStart, yCentreR);
        rightDelayOutPath.lineTo(xVertical, yCentreR);
        rightDelayOutPath.lineTo(xVertical, yRight);
        rightDelayOutPath.lineTo(farX, yRight);
        //rightDelayOutPath = rightDelayOutPath.createPathWithRoundedCorners(14.0f);
        g.strokePath(rightDelayOutPath, juce::PathStrokeType(2.0f));
        

        
        
    }

    void resized () override
    {
        auto area = getLocalBounds().reduced(5.0f);
        auto feedbackKnobArea = area.removeFromBottom (getHeight() * 0.25f);
        
        feedbackSlider->setBounds (feedbackKnobArea);
        leftDelaySlider->setBounds (area.removeFromTop (area.getHeight() * 0.5f).removeFromLeft (getWidth() * 0.8f));
        rightDelaySlider->setBounds (area.removeFromLeft (getWidth() * 0.8f));


    }

private:
    DelayLookAndFeel delayFrameLookAndFeel;
    DelayLookAndFeel feedbackKnobLook;
    juce::Colour windowColour {juce::Colours::white};
    juce::Colour textColour {juce::Colours::black};
    std::unique_ptr<juce::Slider> leftDelaySlider;
    std::unique_ptr<juce::Slider> rightDelaySlider;
    std::unique_ptr<juce::Slider> feedbackSlider;
    
    const float edge {3.0f};
    juce::Font textFont;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayFrame)
};
