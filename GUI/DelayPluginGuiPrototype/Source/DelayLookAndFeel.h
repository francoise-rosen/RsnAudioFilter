/*
  ==============================================================================

    DelayLookAndFeel.h
    Created: 14 Sep 2020 9:36:23am
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

/*
 - l+f for rotary slider (different colours)
 - l+f for textbox of the slider
 - l+f for combobox
 - l+f for button
 
 Inputs:
 - bool : slider vertical gradient on off
 - colour palette
 - slider textbox position
 */


class DelayLookAndFeel : public juce::LookAndFeel_V4
{
public:
    DelayLookAndFeel()
    {}
    
    virtual ~DelayLookAndFeel()
    {}
    
    // SLIDER
    
    void drawRotarySlider(juce::Graphics& g,  int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override
    {
        
        
        
    }
    
private:
    
    
};
