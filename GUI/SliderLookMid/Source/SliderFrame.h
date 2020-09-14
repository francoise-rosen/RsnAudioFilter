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
        fontHeight = getHeight() * 0.05f;
        //customSliderMid.isWithGradient(false);
        setLookAndFeel(&customSliderMid);
        addAndMakeVisible(apple);
        //orange.setLookAndFeel(&customSlider2);
        addAndMakeVisible(orange);

    }

    ~SliderFrame() override
    {
        DBG("Slider Frame distructed\n");
        setLookAndFeel(nullptr);
    
    }

    void paint (juce::Graphics& g) override
    {
        juce::Colour background = juce::Colours::lightcyan.withBrightness(0.75f).withAlpha(0.75f);
        g.fillAll(background);
        customSliderMid.setBackgroundColour(background);
        
        g.setColour(juce::Colours::yellow.withHue(0.90f).withBrightness(0.25f).withAlpha(0.35f));
        
        // DRAW TRIANGLES
        
        auto leftTriangleArea = getLocalBounds().removeFromLeft(getWidth() * 0.5f);
        leftTriangleArea = leftTriangleArea.reduced(leftTriangleArea.getWidth() * 0.25f, leftTriangleArea.getHeight() * 0.2f);
        juce::Path leftTriangle;
        leftTriangle.startNewSubPath(leftTriangleArea.getX(), leftTriangleArea.getY());
        leftTriangle.lineTo(leftTriangleArea.getX(), leftTriangleArea.getBottom());
        leftTriangle.lineTo(leftTriangleArea.getRight(), leftTriangleArea.getBottom());
        leftTriangle.closeSubPath();
        leftTriangle = leftTriangle.createPathWithRoundedCorners(20.0f);
        g.fillPath(leftTriangle);
        //g.strokePath(leftTriangle, juce::PathStrokeType(3.0f));
        
        g.setColour(juce::Colours::yellow.withHue(0.77f).withBrightness(0.25f));
        
        auto rightTriangleArea = getLocalBounds().removeFromRight(getWidth() * 0.75f);
        rightTriangleArea = rightTriangleArea.reduced(rightTriangleArea.getWidth() * 0.2f, rightTriangleArea.getHeight() * 0.1f);
        juce::Path rightTriangle;
        rightTriangle.startNewSubPath(rightTriangleArea.getX(), rightTriangleArea.getY());
        rightTriangle.lineTo(rightTriangleArea.getRight(), rightTriangleArea.getY());
        rightTriangle.lineTo(rightTriangleArea.getRight(), rightTriangleArea.getBottom());
        rightTriangle.closeSubPath();
        rightTriangle = rightTriangle.createPathWithRoundedCorners(25.0f);
        //g.fillPath(rightTriangle);
        g.strokePath(rightTriangle, juce::PathStrokeType(4.0f));
        
        // DRAW LABELS
        
        g.setFont(juce::Font("Monaco", "Bold", fontHeight));
        g.setColour(juce::Colours::white.withAlpha(0.5f));
        
        g.drawFittedText("aMOUNT", leftTriangleArea.reduced(leftTriangleArea.getWidth() * 0.05f), juce::Justification::bottomLeft, 1);
        
        g.drawFittedText("FRe(Q)ueNCy", rightTriangleArea.reduced(rightTriangleArea.getWidth() * 0.05f), juce::Justification::topRight, 1);
        
    }

    void resized() override
    {
        auto sliderArea = getLocalBounds().reduced(edge);
        fontHeight = getHeight() * 0.05f;
        
        apple.setBounds(sliderArea.removeFromLeft(sliderArea.getWidth()/3));
        orange.setBounds(sliderArea);
    }

private:
    juce::Slider apple {juce::Slider::SliderStyle::Rotary, juce::Slider::TextBoxBelow };
    juce::Slider orange {juce::Slider::SliderStyle::Rotary, juce::Slider::TextBoxBelow };
    SliderMidLookAndFeel customSliderMid;
    SliderMidLookAndFeel customSlider2;
    const float edge {3.0f};
    float fontHeight;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderFrame)
};
