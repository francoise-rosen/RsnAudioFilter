/*
  ==============================================================================

    AlphaOneLookAndFeel.h
    Created: 13 Oct 2020 4:41:33pm
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once


class AlphaOneLookAndFeel : public juce::LookAndFeel_V4
{
public:
    AlphaOneLookAndFeel()
    {}
    
    virtual ~AlphaOneLookAndFeel()
    {}
    
    /** LINEAR SLIDER. */
    int getSliderThumbRadius (juce::Slider& slider) override
    {
        return juce::jmin (15.0f, slider.isHorizontal() ? static_cast<float> (slider.getHeight()) * 0.5f : static_cast<float> (slider.getWidth()) * 0.5f);
    }
    
    void drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPos,
                           float minSliderPos,
                           float maxSliderPos,
                           const juce::Slider::SliderStyle style,
                           juce::Slider& slider) override
    {
        /** Cast dimentions. */
        float sliderX = static_cast<float> (x);
        float sliderY = static_cast<float> (y);
        float sliderWidth = static_cast<float> (width);
        float sliderHeight = static_cast<float> (height);
        
        /** Draw a bar slyle slider. */
        if (slider.isBar())
        {
            g.setColour (slider.findColour (juce::Slider::trackColourId));
            /** fill the tracked area */
            g.fillRect (slider.isHorizontal() ? juce::Rectangle<float> (
                                                                        sliderX,
                                                                        sliderY + 0.5f,
                                                                        sliderPos - sliderX,
                                                                        sliderHeight - 1.0f) :
                        juce::Rectangle<float> (
                                                sliderX + 0.5f,
                                                sliderPos,
                                                sliderWidth - 1.0f,
                                                sliderY + (sliderHeight - sliderPos)));
            return;
        }
        
        /** If the slider is not a bar. */
        auto isTwoVal = (style == juce::Slider::SliderStyle::TwoValueHorizontal || (style == juce::Slider::SliderStyle::TwoValueVertical));
        auto isThreeVal = (style == juce::Slider::SliderStyle::ThreeValueHorizontal || (style == juce::Slider::SliderStyle::ThreeValueVertical));
        auto trackWidth = juce::jmin (9.0f, slider.isHorizontal() ? sliderWidth * 0.25f : sliderHeight * 0.25f);
        
        /** Draw background. */
        juce::Point<float> startPoint {slider.isHorizontal() ? sliderX : sliderX + sliderWidth * 0.5f, slider.isHorizontal() ? sliderY + sliderHeight * 0.5f : sliderY + sliderHeight};
        juce::Point<float> endPoint {slider.isHorizontal() ? sliderX + sliderWidth : sliderX + sliderWidth * 0.5f, slider.isHorizontal() ? sliderY + sliderHeight * 0.5f : sliderY};
        juce::Path backgroundTrack;
        backgroundTrack.startNewSubPath (startPoint);
        backgroundTrack.lineTo (endPoint);
        g.strokePath (backgroundTrack, juce::PathStrokeType (trackWidth, juce::PathStrokeType::beveled, juce::PathStrokeType::rounded));
        
        /** Draw value track. */
        juce::Path valueTrack;
        juce::Point<float> minPoint, maxPoint, thumbPoint;
        if (isTwoVal || isThreeVal)
        {
            /** If isTwoVal, then track is between max and min point.
             if isThreeVal, then we need to draw additional track.
             */
            minPoint = { slider.isHorizontal() ? minSliderPos : sliderWidth * 0.5f, slider.isHorizontal() ? sliderHeight * 0.5f : minSliderPos };
            if (isThreeVal)
                thumbPoint = { slider.isHorizontal() ? sliderPos : sliderWidth * 0.5f, slider.isHorizontal() ? sliderHeight * 0.5f : sliderPos };
            maxPoint = { slider.isHorizontal() ? maxSliderPos : sliderWidth * 0.5f, slider.isHorizontal() ? sliderWidth * 0.5f : maxSliderPos };
        }
        else
        {
            /** Linear one point. */
            auto maxX = slider.isHorizontal() ? sliderPos : sliderX + sliderWidth * 0.5f;
            auto maxY = slider.isHorizontal() ? sliderY + sliderHeight * 0.5f : sliderPos;
            minPoint = startPoint;
            maxPoint = { maxX, maxY };
        }
        /** Draw the track. From min to sliderPos */
        auto thumbWidth = juce::jmax (static_cast<float> (getSliderThumbRadius (slider)), trackWidth);
        valueTrack.startNewSubPath (minPoint);
        valueTrack.lineTo (isThreeVal ? thumbPoint : maxPoint);
        g.setColour (slider.findColour (juce::Slider::trackColourId));
        g.strokePath (valueTrack, {trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded});
        /** Thumb. Which to draw? But get a thumb radius anyway. */
        if (! isTwoVal)
        {
            juce::Rectangle<float> thumbRect { thumbWidth, thumbWidth };
            juce::ColourGradient gradient {
                slider.findColour (juce::Slider::thumbColourId),
                isThreeVal ? thumbPoint.getX() : maxPoint.getX(),
                isThreeVal ? thumbPoint.getY() : maxPoint.getY(),
                juce::Colours::yellow.withBrightness (0.5f),
                isThreeVal ? thumbPoint.getX() + thumbWidth * 0.5f : maxPoint.getX() + thumbWidth * 0.5f,
                isThreeVal ? thumbPoint.getY() + thumbWidth * 0.5f : maxPoint.getY() + thumbWidth * 0.5f,
                true
            };
            g.setColour (slider.findColour (juce::Slider::thumbColourId));
            g.setGradientFill (gradient);
            g.fillEllipse (thumbRect.withCentre (isThreeVal ? thumbPoint : maxPoint));
        }
        
        if (isTwoVal || isThreeVal)
        {
            
        }
    }
    
    void drawPointer (juce::Graphics& g, float x, float y, float diameter, juce::Colour& colour, int direction) noexcept
    {
        /** Simple triangle ? */
        juce::Path p;
        p.startNewSubPath (x, y);
        p.lineTo (x + diameter * 0.5f, y - diameter * 0.5f);
        p.lineTo (x - diameter * 0.5f, y - diameter * 0.5f);
        p.closeSubPath();
        g.setColour (colour);
        g.fillPath (p);
    }
    
    /** ROTARY SLIDER. */
    
private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AlphaOneLookAndFeel)
    
};

/** Linear symmetrical slider with tracking colour
    changing from the central position to the right
    and to the left.
    Overrides only drawing linear slider.
    This is to use only from symmetrical Linear (one value) or rotary sliders.
    For Bar, TwoValue, ThreeValue, use another l+f
 */

class AlphaOneSymmetricalSlider : public AlphaOneLookAndFeel
{
public:
    AlphaOneSymmetricalSlider()
    {}
    virtual ~AlphaOneSymmetricalSlider()
    {}
    
    /** LINEAR SLIDER. */
    int getSliderThumbRadius (juce::Slider& slider) override
    {
        return juce::jmin (15.0f, slider.isHorizontal() ? static_cast<float> (slider.getHeight()) * 0.5f : static_cast<float> (slider.getWidth()) * 0.5f);
    }
    
private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AlphaOneSymmetricalSlider)
    
};
