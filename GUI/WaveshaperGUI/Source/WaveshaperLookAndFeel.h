/*
  ==============================================================================

    WaveshaperLookAndFeel.h
    Created: 20 Sep 2020 12:29:48pm
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

/** Error when h + cpp, juce not found or expected class name error. */

class WaveshaperLookAndFeel : public juce::LookAndFeel_V4
{
public:
    WaveshaperLookAndFeel() {}
    virtual ~WaveshaperLookAndFeel() override
    {}
    
    /** Slider functions. */
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveshaperLookAndFeel)
};

/** Slider functions. */
void WaveshaperLookAndFeel::drawRotarySlider (juce::Graphics &g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider &slider)
{
    
}



