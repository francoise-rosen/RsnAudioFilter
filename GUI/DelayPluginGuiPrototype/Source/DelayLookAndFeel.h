/*
  ==============================================================================

    DelayLookAndFeel.h
    Created: 14 Sep 2020 9:36:23am
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once
#include <array>

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
        
        juce::Point<float> centre {(x + width / 2.0f), (y + height / 2.0f)};
        
        /* TOP PLANE */
        float radius = juce::jmin(width * 0.5f, height * 0.5f) * objectScale;
        juce::Point<float> innerPlaneXY {centre.getX() - radius, centre.getY() - radius};
        float diameter = radius * 2.0f;
        float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
        
        /* BOTTOM PLANE = OUTER RIM CIRCLE */
        const float zShiftRatio = 0.067f;
        float xShift = diameter * zShiftRatio;
        float yShift = diameter * zShiftRatio;
        float centreShift = std::sqrt(xShift * xShift + yShift * yShift);
        float rimRadius = radius + centreShift + juce::jmin(xShift, yShift);
        float rimDiameter = rimRadius * 2.0f;
        juce::Point<float> outerPlaneXY {centre.getX() - rimRadius, centre.getY() - rimRadius};
        
        /* The Sides */
        if (withGradient)
        {
            /* set the gradient */
            juce::Colour outerColour {colourPalette[shadow]};
            juce::Colour innerColour {colourPalette[shadow].withBrightness(0.27f)};
            g.setGradientFill({outerColour, outerPlaneXY.getX(), height * 0.5f, innerColour, width * 0.87f, centre.getY(), true});
        }
        else
        {
            g.setColour(colourPalette[shadow]);
        }
        
        /* draw the bottom circle in the centre */
        g.fillEllipse(outerPlaneXY.getX(), outerPlaneXY.getY(), rimDiameter, rimDiameter);
        
        /* shift the top circle, its centre and draw it */
        innerPlaneXY = {innerPlaneXY.getX() + xShift, innerPlaneXY.getY() + yShift};
        g.setColour(colourPalette[object]);
        g.fillEllipse(innerPlaneXY.getX(), innerPlaneXY.getY(), diameter, diameter);
        juce::Point<float> topPlaneCentre {centre.getX() + xShift, centre.getY() + yShift};
        
//        if (withTopPlaneRim)
//        {
//            auto ratio = 0.8f;
//            juce::Point<float> thisRimXY {}
//        }
        
        /* draw a pointer */
        g.setColour(colourPalette[rim]);
        drawSliderThumb(g, topPlaneCentre, radius, angle);
        
        
    }
    
    void setBackgroundColour(const juce::Colour& colour)
    {
        colourPalette[background] = colour;
    }
    
    void setObjectColour(const juce::Colour& colour)
    {
        colourPalette[object] = colour;
    }
    
    void setRimColour(const juce::Colour& colour)
    {
        colourPalette[rim] = colour;
    }
    
    void setShadowColour(const juce::Colour& colour)
    {
        colourPalette[shadow] = colour;
    }
    
private:
    enum ColourTarget {background, object, rim, shadow, numOfTargets};
    std::array<juce::Colour, numOfTargets> colourPalette
    {
        juce::Colours::white.withAlpha(0.5f),
        juce::Colours::blue.withBrightness(0.58f),
        juce::Colours::white,
        juce::Colours::black
    };
    const float objectScale {0.5f}; /* slider's top area relative to entire slider's area */
    bool withGradient {true}; /* sets gradient on / off for the part that's being drawn */
    bool withTopPlaneRim {false};
    bool withBottomPlaneRim {false};
    
    void drawSliderThumb(juce::Graphics& g, juce::Point<float>& centre, const float& radius, const float& angle)
    {
        juce::Path p;
        
        /* thumb dimentions - width and length */
        std::pair<float, float> thumbDim {radius * 0.12f, radius * 0.57f};
        float thumbWidth = radius * 0.112f;
        float thumbLength = radius * 0.57f;
        p.addRoundedRectangle( -thumbWidth, -radius, thumbWidth, thumbLength, juce::jmin(3.0f, thumbWidth * 0.5f), juce::jmin(3.0f, thumbWidth * 0.5f), true, true, false, true);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(centre));
        g.fillPath(p);
    }
    
};
