/*
  ==============================================================================

    AlphaOneLookAndFeel.h
    Created: 13 Oct 2020 4:41:33pm
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

/** TODO:
 AlphaOneLookAndFeel:
    - clean up
    - linear slider round thumb - implement gradient (user defined inner and outer colour)
    - add draw rotary
    - text field label
 
 AlphaOneSymmetricalSlider:
    - clean up
    - colours (track, thumb fill, thumb outer rim)
    - text field label
 
 AlphaOneSymmetricalSlider_V2
    - triangle backround shapes? (three items)
    - triangle rim and fill colours
    - track gradient (hard left - dark orange, hard right - bright silver)
 */

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
        juce::Colour backgroundColour = slider.findColour (juce::Slider::backgroundColourId);
        
        /** Draw a bar slyle slider. */
        if (slider.isBar())
        {
            g.setColour (slider.findColour (juce::Slider::trackColourId));
            /** fill the tracked area */
            g.fillRect (slider.isHorizontal() ?
                        juce::Rectangle<float> (
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
        g.setColour (backgroundColour);
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
            maxPoint = { slider.isHorizontal() ? maxSliderPos : sliderWidth * 0.5f, slider.isHorizontal() ? sliderHeight * 0.5f : maxSliderPos };
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
        /** Thumb. */
        if (! isTwoVal)
        {
            juce::Rectangle<float> thumbRect { thumbWidth, thumbWidth };
            juce::ColourGradient gradient {
                slider.findColour (juce::Slider::thumbColourId),
                isThreeVal ? thumbPoint.getX() : maxPoint.getX(),
                isThreeVal ? thumbPoint.getY() : maxPoint.getY(),
                thumbOuterGColour,
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
            auto sr = juce::jmin (trackWidth, (slider.isHorizontal() ? sliderHeight : sliderWidth) * 0.4f);
            auto pointerColour = slider.findColour (juce::Slider::thumbColourId).withAlpha (0.85f).withHue (0.89f);
            if (slider.isHorizontal())
            {
//                drawPointer (g, minSliderPos - sr,
//                             juce::jmax (0.0f, sliderY + sliderHeight * 0.5f - trackWidth * 2.0f), trackWidth * 2.0f, pointerColour, 2);
//                drawPointer (g, maxSliderPos - trackWidth,
//                             juce::jmin (sliderY + sliderHeight - trackWidth * 2.0f, sliderY + sliderHeight * 0.5f), trackWidth * 2.0f, pointerColour, 0);
                drawPointer (g,
                             minSliderPos - trackWidth,
                             sliderY + sliderHeight * 0.5f - trackWidth * 2.0f,
                             trackWidth * 2.0f,
                             pointerColour,
                             2);
                drawPointer (g,
                             maxSliderPos - trackWidth,
                             sliderY + sliderHeight * 0.5f,
                             trackWidth * 2.0f,
                             pointerColour,
                             0);
            }
            else
            {
                drawPointer (g,
                             juce::jmax (0.0f, sliderX + sliderWidth * 0.5f - trackWidth * 2.0f),
                             minSliderPos - trackWidth,
                             trackWidth * 2.0f,
                             pointerColour, 1);
                drawPointer (g,
                             juce::jmin (sliderX + sliderWidth * 0.5f, sliderX + sliderWidth - 2.0f * trackWidth),
                             maxSliderPos - sr,
                             trackWidth * 2.0f,
                             pointerColour,
                             3);
            }
        }
    }
    
    void drawPointer (juce::Graphics& g, float x, float y, float diameter, juce::Colour& colour, int direction) noexcept
    {
        /** Simple triangle ? */
        juce::Path p;
        p.startNewSubPath (x + diameter * 0.5f, y);
        p.lineTo (x + diameter, y + diameter * 0.79f);
        p.lineTo (x, y + diameter * 0.79f);
        p.closeSubPath();
        p.applyTransform (juce::AffineTransform::rotation (static_cast<float> (direction) * juce::MathConstants<float>::halfPi, x + diameter * 0.5f, y + diameter * 0.5f));
        g.setColour (colour);
        g.fillPath (p);

        juce::Path line;
        line.startNewSubPath (x + diameter * 0.5f, y + diameter * 0.4f);
        line.lineTo (x + diameter * 0.5f, y + diameter);
        line.closeSubPath();
        line.applyTransform (juce::AffineTransform::rotation (static_cast<float> (direction) * juce::MathConstants<float>::halfPi, x + diameter * 0.5f, y + diameter * 0.5f));
        g.strokePath (line, { juce::jmin (2.0f, diameter * 0.25f), juce::PathStrokeType::curved, juce::PathStrokeType::rounded });
    }
    
//    void drawPointer (juce::Graphics& g, float x, float y, float diameter, juce::Colour& colour, int direction) noexcept
//    {
//        /** Centre. */
//        g.setColour (juce::Colours::red);
//        g.fillEllipse (juce::Rectangle<float> {2.0f, 2.0f}.withCentre ({x + diameter * 0.5f, y + diameter * 0.5f}));
//
//        g.setColour (juce::Colours::yellow.withAlpha (0.2f));
//        g.fillEllipse (juce::Rectangle<float> {diameter, diameter}.withCentre ({x + diameter * 0.5f, y + diameter * 0.5f}));
//
//    }
    void setThumbOuterGradientColour (juce::Colour colour)
    {
        thumbOuterGColour = colour;
    }
    
    /** ROTARY SLIDER. */
    
private:
    juce::Colour thumbOuterGColour { juce::Colours::yellow.withBrightness (0.5f) };
    juce::Colour pointerOuterGColour { juce::Colours::black };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AlphaOneLookAndFeel)
    
};

/** Linear symmetrical slider with tracking colour
    changing from the central position to the right
    and to the left.
    Overrides only drawing linear slider.
    This is to use only from symmetrical Linear (one value) or rotary sliders.
    For Bar, TwoValue, ThreeValue, use another l+f
 */

/** Linear and rotary symmetrical slider. */
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
    
    /** Only horizontal or vertical one value slider. */
    void drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPos,
                           float minSliderPos,
                           float maxSliderPos,
                           const juce::Slider::SliderStyle style,
                           juce::Slider& slider) override
    {
        jassert (slider.isHorizontal() || slider.isVertical());
        /** Cast dimentions. */
        float sliderX = static_cast<float> (x);
        float sliderY = static_cast<float> (y);
        float sliderWidth = static_cast<float> (width);
        float sliderHeight = static_cast<float> (height);
        float trackWidth = juce::jmin (9.0f, slider.isHorizontal() ? sliderHeight * 0.25f : sliderWidth * 0.25f);
        
        /** Draw background. */
        juce::Colour backgroundColour = slider.findColour (juce::Slider::backgroundColourId);
        juce::Point<float> startPos { slider.isHorizontal() ? sliderX : sliderX + sliderWidth * 0.5f, slider.isHorizontal() ? sliderY + sliderHeight * 0.5f : sliderY + sliderHeight};
        juce::Point<float> endPos { slider.isHorizontal() ? sliderX + sliderWidth : sliderX + sliderWidth * 0.5f, slider.isHorizontal() ? sliderY + sliderHeight * 0.5f : sliderY };
        juce::Point<float> midPos { sliderX + sliderWidth * 0.5f, sliderY + sliderHeight * 0.5f };
        juce::Path background;
        g.setColour (backgroundColour);
        background.startNewSubPath (startPos);
        background.lineTo (endPos);
        background.closeSubPath();
        g.strokePath (background, {trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });
        
        /** Draw track.
            This is filled from StartPos till SliderPos for simple linear slider.
         */
        juce::Colour trackColour = slider.findColour (juce::Slider::trackColourId);
        juce::Point<float> minPoint = startPos;
        juce::Point<float> maxPoint { slider.isHorizontal() ? sliderPos : sliderX + sliderWidth * 0.5f, slider.isHorizontal() ? sliderY + sliderHeight * 0.5f : sliderPos };
//        juce::Path trackPath;
//        trackPath.startNewSubPath (minPoint);
//        trackPath.lineTo (maxPoint);
//        trackPath.closeSubPath();
//        g.setColour (trackColour);
//        g.strokePath (trackPath, {trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded});
        
        /** Draw track from mid point. */
        juce::Path track;
        track.startNewSubPath (midPos);
        track.lineTo (maxPoint);
        track.closeSubPath();
        g.setColour (trackColour);
        g.strokePath (track, {trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded});
        
        /** Draw thumb. */
        auto thumbWidth = juce::jmax (static_cast<float> (getSliderThumbRadius (slider)), trackWidth);
        auto sr = juce::jmin (trackWidth, slider.isHorizontal() ? sliderHeight : sliderWidth * 0.4f);
        juce::Colour thumbColour = slider.findColour (juce::Slider::thumbColourId);
        /** Directions:
            0 - points up, 1 - points to the right, 2, points up, 3 points left
         */
        
        /** Draw circlular thumb
         */
        juce::Rectangle<float> thumbArea {thumbWidth * 0.2f, thumbWidth * 0.2f};
        g.setColour (thumbColour);
        g.fillEllipse (thumbArea.withCentre (maxPoint));
        g.fillEllipse (thumbArea.withCentre (midPos));
        if (slider.isHorizontal() )
        {
            /** lower thumb. */
//            drawThumbLinearTri (g,
//                                maxPoint.getX() - sr,
//                                maxPoint.getY(),
//                                trackWidth * 2.0f,
//                                thumbColour,
//                                0);
            /** Upper thumb. */
            drawThumbLinearTri (g,
                                maxPoint.getX() - sr,
                                maxPoint.getY() - sr * 2,
                                trackWidth * 2.0f,
                                thumbColour,
                                2);

        }
        else
        {
            /** Thumb poinint to the left. */
            drawThumbLinearTri (g, maxPoint.getX(),
                                maxPoint.getY() - sr,
                                trackWidth * 2.0f,
                                thumbColour,
                                3);

        }
        
    }
    
private:
    
    void drawThumbLinearTri (juce::Graphics& g, float x, float y, float diameter, juce::Colour& colour, int direction ) noexcept
    {
        g.setColour (juce::Colours::red);
        juce::Point<float> pivot {x + diameter * 0.5f, y + diameter * 0.5f};
        g.fillEllipse (juce::Rectangle<float> {2.0f, 2.0f}.withCentre (pivot));
        /** Simple triangle ? */
        g.setColour (juce::Colours::yellow.withAlpha (0.25f));
        g.fillEllipse (juce::Rectangle<float> {diameter, diameter}.withCentre (pivot));
        
        g.setColour (juce::Colours::blue);
        juce::Path tri;
        tri.startNewSubPath (x + diameter * 0.5f, y);
        tri.lineTo (x + diameter, y + diameter * 0.75f);
        tri.lineTo (x, y + diameter * 0.75f);
        tri.closeSubPath();
        tri.applyTransform (juce::AffineTransform::rotation (static_cast<float> (direction) * juce::MathConstants<float>::halfPi, pivot.getX(), pivot.getY()));
        g.strokePath (tri, juce::PathStrokeType (3.0f));
 

    }
    
//    void drawThumbLinearTri (juce::Graphics& g, float x, float y, float diameter, juce::Colour& colour, int direction ) noexcept
//    {
//        /** Simple triangle ? */
//        juce::Path p;
//        p.startNewSubPath (x + diameter * 0.5f, y);
//        p.lineTo (x + diameter, y + diameter * 0.5f);
//        p.lineTo (x, y + diameter * 0.5f);
//        p.closeSubPath();
//        p.applyTransform (juce::AffineTransform::rotation (static_cast<float> (direction) * juce::MathConstants<float>::halfPi, x + diameter * 0.5f, y + diameter * 0.5f));
//        g.setColour (colour);
//        g.fillPath (p);
//
//    }
    
    void drawThumbRotary () noexcept
    {
        
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AlphaOneSymmetricalSlider)
    
};

class AlphaOneSymmetricalSlider_V2 : public AlphaOneSymmetricalSlider
{
public:
    AlphaOneSymmetricalSlider_V2()
    {}
    virtual ~AlphaOneSymmetricalSlider_V2()
    {}
};
