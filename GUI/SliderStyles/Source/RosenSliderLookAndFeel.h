/*
  ==============================================================================

    RosenSliderLookAndFeel.h
    Created: 10 Oct 2020 10:27:43am
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

class RosenSliderLookAndFeel : public juce::LookAndFeel_V4
{
public:
    RosenSliderLookAndFeel()
    {}
    
    virtual ~RosenSliderLookAndFeel()
    {}
    
    /** Rotary. */
    /** Draw a thumb. */
    
    /** Linear. */
    /** Get/set a thimb size. */
    enum class ThumbShape { ellipse, circle, roundedRect, rhombus, sixPointPoly, eightPointPoly };
    int getSliderThumbRadius (juce::Slider& slider) override
    {
        return juce::jmin (12.0f, slider.isHorizontal() ? static_cast<float> (slider.getHeight()) * 0.5f : static_cast<float> (slider.getWidth()) * 0.5f);
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
        auto trackWidth = juce::jmin (5.0f, slider.isHorizontal() ? sliderWidth * 0.25f : sliderHeight * 0.25f);
        
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
        auto thumbWidth = juce::jmax (static_cast<float> (getSliderThumbRadius (slider)), trackWidth * 2.5f);
        valueTrack.startNewSubPath (minPoint);
        valueTrack.lineTo (isThreeVal ? thumbPoint : maxPoint);
        g.setColour (slider.findColour (juce::Slider::trackColourId));
        g.strokePath (valueTrack, {trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded});
        /** Thumb. Which to draw? But get a thumb radius anyway. */
        if (! isTwoVal)
        {
            juce::Rectangle<float> thumbRect {
                slider.isHorizontal() ? thumbWidth : thumbWidth * 2.5f,
                slider.isHorizontal() ? thumbWidth * 2.5f : thumbWidth
                
            };
            g.setColour (slider.findColour (juce::Slider::thumbColourId));
            /** Draw a thumb depending on the Thumb Shape.
                Perhapse use polymorphism instead.
             */
            if (linearSliderThumbShape == ThumbShape::ellipse)
            {
                g.fillEllipse (thumbRect.withCentre (isThreeVal ? thumbPoint : maxPoint));
            }
            else if (linearSliderThumbShape == ThumbShape::circle)
            {
                g.fillEllipse (juce::Rectangle<float> {thumbWidth, thumbWidth}.withCentre (isThreeVal ? thumbPoint : maxPoint));
            }
            else if (linearSliderThumbShape == ThumbShape::roundedRect)
            {
                g.fillRoundedRectangle (thumbRect.withCentre (isThreeVal ? thumbPoint : maxPoint ), 5.0f);
                g.setColour (slider.findColour (juce::Slider::thumbColourId).darker());
                g.drawRoundedRectangle(thumbRect.withCentre (isThreeVal ? thumbPoint : maxPoint), 5.0f, 2.0f);
                /** Centre line. */
                if (slider.isHorizontal())
                {
                    g.fillRoundedRectangle (thumbRect.withSize (thumbRect.getWidth() * 0.2f, thumbRect.getHeight() * 0.75f).withCentre (isThreeVal ? thumbPoint : maxPoint), 2.0f);
                }
                else
                {
                    g.fillRoundedRectangle (thumbRect.withSize(thumbRect.getWidth() * 0.75f, thumbRect.getHeight() * 0.2f).withCentre (isThreeVal ? thumbPoint : maxPoint), 2.0f);
                }
            }
            else if (linearSliderThumbShape == ThumbShape::rhombus)
            {
                g.fillEllipse (juce::Rectangle<float> {thumbWidth * 1.2f, thumbWidth * 1.3f}.withCentre (isThreeVal ? thumbPoint : maxPoint));
                //g.setColour (slider.findColour (juce::Slider::thumbColourId).darker());
                juce::Path rhombus;
                if (slider.isHorizontal())
                {
                    rhombus.startNewSubPath (isThreeVal ? thumbPoint.getX() : maxPoint.getX(), isThreeVal ? thumbPoint.getY() - thumbRect.getHeight() * 0.5f : maxPoint.getY() - thumbRect.getHeight() * 0.5f);
                    rhombus.lineTo (isThreeVal ? thumbPoint.getX() - thumbRect.getWidth() * 0.35f : maxPoint.getX() - thumbRect.getWidth() * 0.35f, isThreeVal ? thumbPoint.getY() : maxPoint.getY());
                    rhombus.lineTo (isThreeVal ? juce::Point<float> { thumbPoint.getX(), thumbPoint.getY() + thumbRect.getHeight() * 0.5f } : juce::Point<float> { maxPoint.getX(), maxPoint.getY() + thumbRect.getHeight() * 0.5f });
                    rhombus.lineTo (isThreeVal ? juce::Point<float> { thumbPoint.getX() + thumbRect.getWidth() * 0.35f, thumbPoint.getY()} : juce::Point<float> { maxPoint.getX() + thumbRect.getWidth() * 0.35f, maxPoint.getY()});
                    rhombus.closeSubPath();
                    g.strokePath (rhombus, {4.0f, juce::PathStrokeType::curved, juce::PathStrokeType::square});
                    g.setColour (juce::Colours::silver.darker());
                    juce::Path pointer;
                    pointer.startNewSubPath (isThreeVal ? thumbPoint.getX() : maxPoint.getX(), isThreeVal ? thumbPoint.getY() - thumbRect.getHeight() * 0.34f : maxPoint.getY() - thumbRect.getHeight() * 0.34f);
                    pointer.lineTo (isThreeVal ? thumbPoint.getX() : maxPoint.getX(), isThreeVal ? thumbPoint.getY() + thumbRect.getHeight() * 0.34f : maxPoint.getY() + thumbRect.getHeight() * 0.34f);
                    g.strokePath (pointer, { 2.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });
                }
                
            }
            
        }
    }
    
    void setThumbShape (ThumbShape thumbShape)
    {
        linearSliderThumbShape = thumbShape;
    }
    

private:
    ThumbShape linearSliderThumbShape { ThumbShape::roundedRect };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RosenSliderLookAndFeel)
    
};


