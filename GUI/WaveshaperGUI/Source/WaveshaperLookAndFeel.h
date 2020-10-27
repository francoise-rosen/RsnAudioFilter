/*
  ==============================================================================

    RosenAlphaOneLookAndFeel
    WaveshaperLookAndFeel.h
    Created: 20 Sep 2020 12:29:48pm
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

/** Error when h + cpp, juce not found or expected class name error. */

/** Rotrary slider types:
    1. Big slider (default slider + inner and outer rim, perhaps different thumb)
    2. Default slider implemented in WaveshaperLookAndFeel - 3 coulours
    3. Small slider - 2 colours
 */

//================================================================================

class WaveshaperLookAndFeel : public juce::LookAndFeel_V4
{
public:
    WaveshaperLookAndFeel()
    {}
    virtual ~WaveshaperLookAndFeel() override
    {}
    
    enum class OutlineType {ellipse, arcNormal, arcWithArrows, arcWithCornersOut, arcWithCornersIn, arcThreePointerEmpty, arcThreePointerFilled, noOutline};

    /** Slider functions. */
    //================================================================================
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
    
    int getSliderThumbRadius (juce::Slider& slider) override;
    void drawLinearSlider (juce::Graphics &, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle, juce::Slider &) override;
    
//    void drawLinearSliderBackground
    
    
    /** Test this (unit test).
        The drawing may overcomplicate the drawRotary method.
        Delegate drowing the arc to other methods?
     */
    void setOutlineType (OutlineType outlineType)
    {
        localOutlineType = outlineType;
    }
    
    /** If outline is not visible, only sliderFill and sliderThumb
     are visible, but outer body (outline is still drown, it's just transparent.
     */
    
    bool isOutlineVisible() const
    {
        return outlineVisible;
    }
    
    void setOutlineVisibile (bool isVisible)
    {
        outlineVisible = isVisible;
    }
    
    //================================================================================
    /** Buttons. */
    
    
    //================================================================================
    /** ComboBox, Text etc. */
    
protected:
    /**  Draw Rotary Slider thumb.
     Make it protected? a virtual? or make it helper, unless I want to use
     different thumbs in derived looks
     */
    virtual void drawRotaryThumb (juce::Graphics& g, const juce::Point<float> centre, const float& radius, const float& angle)
    {
        /** Thumb dimentions (rectangle). */
        const float thumbWidth = radius * 0.27f;
        const float thumbHeight = radius * 0.33f;
        
        juce::Path p;
        p.addRectangle (-thumbWidth * 0.5, -radius * 0.87f, thumbWidth, thumbHeight);
        p.applyTransform (juce::AffineTransform::rotation (angle).translated (centre));
        g.fillPath (p);
    }


private:
    const float edge {5.0f};
    const float sliderOuterRimScaleFactor {0.92f};
    bool outlineVisible {true};
    OutlineType localOutlineType {OutlineType::arcNormal};

    
    /** Draw Linear Slider thumb. */
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveshaperLookAndFeel)
};

/** Slider functions.
    This is a middle rotary slider with outer rim, inner body, outer body, and a thumb.
    This is a plane version, so the inner body is placed right in the centre of the
    outer body.
 */
inline void WaveshaperLookAndFeel::drawRotarySlider (juce::Graphics &g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider &slider)
{
    auto fill = slider.findColour (juce::Slider::rotarySliderFillColourId);
    auto outline = slider.findColour (juce::Slider::rotarySliderOutlineColourId);
    juce::Point<float> centre {x + width * 0.5f, y + height * 0.5f};
    auto area = juce::Rectangle<int> (x, y, width, height).toFloat().reduced (edge);
    auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
    
    /** Outer body and outline. */
    const float outerRadius = juce::jmin (area.getWidth() * 0.5f, area.getHeight() * 0.5f) * sliderOuterRimScaleFactor;
    const float rimWidth = 2.0f;
    juce::Point<float> outerRimXY {centre.getX() - outerRadius, centre.getY() - outerRadius};
    
    /** Inner body. */
    const float innerRadius = outerRadius * 0.69f;
    juce::Point<float> innerRimXY {centre.getX() - innerRadius, centre.getY() - innerRadius};
    
    if (outlineVisible)
    {
        /**
           - I want to draw starting and ending at different angles from this in input
           - I want to use outline type that can be set by caller
           - delegate this to other functions, maybe helpers?
         */
        juce::Path outerArc;
        outerArc.addCentredArc (centre.getX(),
                                centre.getY(),
                                outerRadius,
                                outerRadius,
                                0.0f,
                                rotaryStartAngle,
                                rotaryEndAngle,
                                true);
        g.setColour (outline);
        g.strokePath (outerArc, juce::PathStrokeType (rimWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    }
    
    g.setColour (fill);
    g.fillEllipse (innerRimXY.getX(), innerRimXY.getY(), innerRadius * 2.0f, innerRadius * 2.0f);
    
    g.setColour (slider.findColour (juce::Slider::thumbColourId));
    drawRotaryThumb (g, centre, outerRadius, angle);
    

}

inline int WaveshaperLookAndFeel::getSliderThumbRadius (juce::Slider& slider)
{
    return juce::jmin (12, slider.isHorizontal() ? static_cast<int> ((float) slider.getHeight() * 0.75f)
                 : static_cast<int> ((float) slider.getWidth()  * 0.75f));
}

inline void WaveshaperLookAndFeel::drawLinearSlider (juce::Graphics &, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, const juce::Slider::SliderStyle, juce::Slider &)
{
    /** At first only Horizonal and Vertical Linear sliders. */
    
    
}

//================================================================================
/** Custom L+F for symmetrical knob. */

class SymmetricalRotaryLookAndFeel : public WaveshaperLookAndFeel
{
public:
    SymmetricalRotaryLookAndFeel() {}
    virtual ~SymmetricalRotaryLookAndFeel() {}
    
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle,
                           float rotaryEndAngle, juce::Slider& slider) override
    {
        
        auto fill = slider.findColour (juce::Slider::rotarySliderFillColourId);
        auto outline = slider.findColour (juce::Slider::rotarySliderOutlineColourId);
        juce::Point<float> centre {x + width * 0.5f, y + height * 0.5f};
        auto area = juce::Rectangle<int> (x, y, width, height).toFloat().reduced (edge);
        auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
        
        /** Outer body. */
        const float outerRadius = juce::jmin (area.getWidth() * 0.5f, area.getHeight() * 0.5f) * sliderOuterRimScaleFactor;
        const float rimWidth = 2.0f;
        juce::Point<float> outerRimXY {centre.getX() - outerRadius, centre.getY() - outerRadius};
        
        /** Inner body. */
        const float innerRadius = outerRadius * 0.69f;
        juce::Point<float> innerRimXY {centre.getX() - innerRadius, centre.getY() - innerRadius};
        
        g.setColour (outline);
        // g.drawEllipse(outerRimXY.getX(), outerRimXY.getY(), outerRadius * 2, outerRadius * 2, 3.0f);
        juce::Path outerArc;
        outerArc.addCentredArc (centre.getX(),
                                centre.getY(),
                                outerRadius,
                                outerRadius,
                                0.0f,
                                rotaryStartAngle,
                                rotaryEndAngle,
                                true);
        g.strokePath (outerArc, juce::PathStrokeType (rimWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        juce::Path middleLine;
        middleLine.startNewSubPath (centre.getX(), centre.getY() - outerRadius);
        middleLine.lineTo (centre.getX(), y);
        g.strokePath (middleLine, juce::PathStrokeType (rimWidth));
        
        g.setColour (fill);
        g.fillEllipse (innerRimXY.getX(), innerRimXY.getY(), innerRadius * 2.0f, innerRadius * 2.0f);
        
        g.setColour (slider.findColour (juce::Slider::thumbColourId));
        drawRotaryThumb (g, centre, outerRadius, angle);

    }
    
private:
    const float edge {5.0f};
    OutlineType localOutlineType {OutlineType::arcNormal};
    float sliderOuterRimScaleFactor {0.92f};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SymmetricalRotaryLookAndFeel)
};


//================================================================================
/** Custom L+F for the big rotary slider. */

class RotaryBigLookAndFeel : public WaveshaperLookAndFeel
{
    
};

//================================================================================
/** Custom L+F for the symmetrical linear slider. */
/** Define the default colours and shapes.
    Provide functions for user to customize those.
 */
class SymmetricalLinearLookAndFeel : public WaveshaperLookAndFeel
{
public:
    SymmetricalLinearLookAndFeel()
    {}
    ~SymmetricalLinearLookAndFeel()
    {}
    
    enum class PointerFill { NoFill, Fill, FillGradient };
    enum class PointerFillType { OneColour, Triangles, Pencil, Gradient };
    enum class PointerType { Triangle, Circle, Rectangle, Arrow };
    
    /** LINEAR SLIDER. */
    
    /** Check if the input is legit? */
    void setSliderThumbRadius (const float& newRadius)
    {
        if (newRadius < 0.0f)
            sliderThumbRadius = 0.0f;
        sliderThumbRadius = newRadius;
    }
    
    int getSliderThumbRadius (juce::Slider& slider) override
    {
        return juce::jmin (sliderThumbRadius, slider.isHorizontal() ? static_cast<float> (slider.getHeight()) * 0.5f : static_cast<float> (slider.getWidth()) * 0.5f);
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
        float trackWidth = juce::jmin (15.0f, slider.isHorizontal() ? sliderHeight * 0.2f : sliderWidth * 0.2f);
        
        juce::Colour backgroundColour = slider.findColour (juce::Slider::backgroundColourId);
        juce::Point<float> startPos { slider.isHorizontal() ? sliderX : sliderX + sliderWidth * 0.5f, slider.isHorizontal() ? sliderY + sliderHeight * 0.5f : sliderY + sliderHeight};
        juce::Point<float> endPos { slider.isHorizontal() ? sliderX + sliderWidth : sliderX + sliderWidth * 0.5f, slider.isHorizontal() ? sliderY + sliderHeight * 0.5f : sliderY };
        juce::Point<float> midPos { sliderX + sliderWidth * 0.5f, sliderY + sliderHeight * 0.5f };
        juce::Point<float> minPoint = startPos;
        juce::Point<float> maxPoint { slider.isHorizontal() ? sliderPos : sliderX + sliderWidth * 0.5f, slider.isHorizontal() ? sliderY + sliderHeight * 0.5f : sliderPos };
        
        /** Symmetrical bar. */
        if (slider.isBar())
        {
            g.setColour (slider.findColour (juce::Slider::trackColourId));
            if (slider.isHorizontal())
            {
                g.fillRect ( (midPos.getX() >= maxPoint.getX() ) ?
                            juce::Rectangle<float> {
                                maxPoint.getX(),
                                sliderY + 0.5f,
                                midPos.getX() - maxPoint.getX(),
                                sliderHeight - 1.0f
                            }                                  :
                            juce::Rectangle<float> {
                                midPos.getX(),
                                sliderY + 0.5f,
                                maxPoint.getX() - midPos.getX(),
                                sliderHeight - 1.0f
                            }
                            );
            }
            else
            {
                g.fillRect ( (midPos.getY() >= maxPoint.getY() ) ?
                            juce::Rectangle<float> {
                                sliderX + 0.5f,
                                midPos.getY(),
                                sliderWidth - 1.0f,
                                midPos.getY() - maxPoint.getY()
                            }                                  :
                            juce::Rectangle<float> {
                                sliderX + 0.5f,
                                maxPoint.getY(),
                                sliderWidth - 1.0f,
                                maxPoint.getY() - midPos.getY()
                            }
                            );
            }
            return;
        };
        
        /** Draw background. */
        
        juce::Path background;
        g.setColour (backgroundColour);
        background.startNewSubPath (startPos);
        background.lineTo (endPos);
        background.closeSubPath();
        g.strokePath (background, {trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });
        
        /** Draw track.
         This is filled from StartPos till SliderPos for simple linear slider.
         */
        drawTrackGradient(g, slider, trackWidth, startPos, midPos, maxPoint, endPos);
        
        /** Draw thumb. */
        auto thumbWidth = juce::jmax (static_cast<float> (getSliderThumbRadius (slider)), trackWidth);
        auto sr = juce::jmin (trackWidth, slider.isHorizontal() ? sliderHeight : sliderWidth * 0.4f);
        
        /** Colours:
         1. colour - the colour of triangle rim (thumbColourId)
         2. pivotColour - same as the color of the dot in the centre of the track
         3. paleCircleColourGradient - the colour of the circular shadow (colour of area background?). The origin is the same colour as midPoint and maxPoint, the outer colour can be background area colour with alpha 0.25f, so that it'll merge in the background around the triangle, but still will be visible both on the track and on the slider's background
         
         */
        juce::Colour thumbColour = slider.findColour (juce::Slider::thumbColourId);
        /** Directions:
         0 - points up, 1 - points to the right, 2, points up, 3 points left
         */
        
        /** Draw circlular thumb
         */
        juce::Rectangle<float> thumbArea {thumbWidth * 0.2f, thumbWidth * 0.2f};
        g.setColour (thumbColour);
        //g.fillEllipse (thumbArea.withCentre (maxPoint));
        g.fillEllipse (thumbArea.withCentre (midPos));
        if (slider.isHorizontal() )
        {
     
            if (pointerType == PointerType::Triangle)
            {
                drawThumbLinearTri (g,
                                    maxPoint.getX() - sr * 1.25f,
                                    maxPoint.getY() - sr * 2.5f,
                                    trackWidth * 2.5f,
                                    thumbColour,
                                    2);
            }
            else if (pointerType == PointerType::Arrow)
            {
                drawThumbLinearArrow(g, maxPoint.getX() - sr,
                                     maxPoint.getY() - sr * 2.0f,
                                     trackWidth * 2.0f,
                                     thumbColour,
                                     2);
            }
            else if (pointerType == PointerType::Circle)
            {
                drawThumbLinearCircle(g,
                                      maxPoint.getX(),
                                      maxPoint.getY(),
                                      trackWidth * 2.0f,
                                      thumbColour);
            }
            else
            {
                /** Rectangle. */
            }
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
        //        g.setColour (thumbColour);
        //        g.fillEllipse (thumbArea.withCentre (maxPoint));
    }
    
    void setThumbTriColour (const juce::Colour newColour)
    {
        linearSliderThumbTriColour = newColour;
    }
    
    void setLinearSliderThumbOuterRimColour (const juce::Colour newColour)
    {
        linearSliderThumbOuterRimColour = newColour.withAlpha (0.25f);
    }
    
    void setPointerFill (PointerFill newFillFlag)
    {
        pointerFill = newFillFlag;
    }
    
    void setPointerType (PointerType newPointerType)
    {
        pointerType = newPointerType;
    }
    
private:
    float sliderThumbRadius {15.0f};
    juce::Colour linearSliderThumbTriColour {juce::Colours::white.withAlpha (0.75f)};
    juce::Colour linearSliderThumbTriFill {juce::Colours::black};
    juce::Colour linearSliderThumbOuterRimColour {juce::Colours::silver.withAlpha (0.2f)};
    PointerFill pointerFill { PointerFill::Fill };
    PointerFillType pointerFillType { PointerFillType::Triangles };
    PointerType pointerType { PointerType::Triangle };
    
    void drawTrackGradient (juce::Graphics& g, juce::Slider& slider, const float& trackWidth, const juce::Point<float> startPos, const juce::Point<float>& midPos, const juce::Point<float>& maxPoint, const juce::Point<float>& endPos)
    {
        juce::Colour trackColour = slider.findColour (juce::Slider::trackColourId);
        /** Draw track from mid point. */
        juce::Path track;
        track.startNewSubPath (midPos);
        track.lineTo (maxPoint);
        track.closeSubPath();
        g.setColour (trackColour);
        g.strokePath (track, {trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded});
    }
    
    void drawThumbLinearTri (juce::Graphics& g, float x, float y, float diameter, juce::Colour& colour, int direction ) noexcept
    {
        
        juce::Point<float> pivot {x + diameter * 0.5f, y + diameter * 0.5f};
        
        /** Pale circle. This is to make a circular shadow on
         the sides of the triangle.
         */
        
        if (pointerFill != PointerFill::FillGradient)
        {
            g.setColour (linearSliderThumbOuterRimColour);
            g.fillEllipse (juce::Rectangle<float> {diameter, diameter}.withCentre (pivot));
        }
        
        juce::Path tri;
        tri.startNewSubPath (x + diameter * 0.5f, y);
        tri.lineTo (x + diameter * 0.87f, y + diameter * 0.75f);
        tri.lineTo (x + diameter * 0.13f, y + diameter * 0.75f);
        tri.closeSubPath();
        tri.applyTransform (juce::AffineTransform::rotation (static_cast<float> (direction) * juce::MathConstants<float>::halfPi, pivot.getX(), pivot.getY()));
        
        /** 1. Filled with colour 2. Filled with gradient 3. No fill */
        if (pointerFill == PointerFill::Fill)
        {
            g.setColour (linearSliderThumbTriFill);
            g.fillPath (tri);
        }
        
        else if (pointerFill == PointerFill::FillGradient)
        {
            juce::ColourGradient gradient
            {
                linearSliderThumbTriFill,
                pivot.getX(),
                pivot.getY(),
                linearSliderThumbOuterRimColour,
                pivot.getX() + diameter * 0.25f,
                pivot.getY() + diameter * 0.25f,
                true
            };
            g.setGradientFill (gradient);
            g.fillEllipse (juce::Rectangle<float> {diameter, diameter}.withCentre (pivot));
        }
        
        /** Draw inner triangle. */
        if (pointerFillType == PointerFillType::Triangles)
        {
            juce::Path innerBottomTri;
            innerBottomTri.startNewSubPath(pivot.getX(), pivot.getY() - diameter * 0.25f);
            innerBottomTri.lineTo (x + diameter * 0.87f, y + diameter * 0.75f);
            innerBottomTri.lineTo (x + diameter * 0.13f, y + diameter * 0.75f);
            innerBottomTri.closeSubPath();
            innerBottomTri.applyTransform (juce::AffineTransform::rotation (static_cast<float> (direction) * juce::MathConstants<float>::halfPi, pivot.getX(), pivot.getY()));
            //g.setColour (juce::Colours::red.withAlpha (0.5f));
            g.setColour (juce::Colours::blue.withBrightness (0.3f));
            g.fillPath (innerBottomTri);
            
            juce::Path innerTri;
            innerTri.startNewSubPath (pivot);
            innerTri.lineTo (x + diameter * 0.87f, y + diameter * 0.75f);
            innerTri.lineTo (x + diameter * 0.13f, y + diameter * 0.75f);
            innerTri.closeSubPath();
            innerTri.applyTransform (juce::AffineTransform::rotation (static_cast<float> (direction) * juce::MathConstants<float>::halfPi, pivot.getX(), pivot.getY()));
            if (linearSliderThumbTriFill != juce::Colours::black)
                g.setColour (linearSliderThumbTriFill.darker().darker());
            else
                //g.setColour (juce::Colours::orange.withAlpha (0.15f));
                g.setColour (juce::Colours::blue.withBrightness (0.2f));
            g.fillPath (innerTri);
        }
        
        /** Undeveloped. */
        else if (pointerFillType == PointerFillType::Pencil)
        {
            g.setColour (juce::Colours::white);
            g.fillPath (tri);
            
            juce::Path innerTri;
            innerTri.startNewSubPath (x + diameter * 0.5f, y);
            innerTri.lineTo (x + diameter * 0.25f, y + diameter * 0.25f);
            innerTri.lineTo (x + diameter * 0.75f, y + diameter * 0.25f);
            innerTri.closeSubPath();
            innerTri.applyTransform (juce::AffineTransform::rotation (static_cast<float> (direction) * juce::MathConstants<float>::halfPi, pivot.getX(), pivot.getY()));
            g.setColour (juce::Colours::white);
            g.fillPath (innerTri);
        }
        
        /** Set colour for the triangle. */
        
        g.setColour (linearSliderThumbTriFill.withAlpha (0.5f));
        g.strokePath (tri, {diameter * 0.15f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded});
        g.setColour (linearSliderThumbTriColour);
        g.strokePath (tri, {diameter * 0.05f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded});
        
        //        g.setColour (linearSliderThumbTriColour);
        //        g.fillEllipse (juce::Rectangle<float> {2.0f, 2.0f}.withCentre (pivot));
        
    }
    
    void drawThumbLinearArrow (juce::Graphics& g, float x, float y, float diameter, juce::Colour& colour, int direction ) noexcept
    {
        juce::Point<float> pivot {x + diameter * 0.5f, y + diameter * 0.5f};
        /** Draw a pointer. */
        juce::Path tri;
        tri.startNewSubPath (x + diameter * 0.5f, y);
        tri.lineTo (x + diameter * 0.81f, y + diameter * 0.75f);
        tri.lineTo (x + diameter * 0.19f, y + diameter * 0.75f);
        tri.closeSubPath();
        tri.applyTransform (juce::AffineTransform::rotation (static_cast<float> (direction) * juce::MathConstants<float>::halfPi, pivot.getX(), pivot.getY()));
        g.setColour (juce::Colours::darkorange.darker());
        g.fillPath (tri);
        g.setColour (linearSliderThumbTriFill.withAlpha (1.0f));
        g.strokePath (tri, {3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded});
        g.setColour (juce::Colours::darkblue.withAlpha (0.75f));
        g.strokePath (tri, {2.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded});
        
        /** Draw an arrow outer path. */
        juce::Path arrowLine;
        arrowLine.startNewSubPath (x + diameter * 0.5f, y + diameter * 0.85f);
        arrowLine.lineTo (x + diameter * 0.5f, y + diameter);
        arrowLine.closeSubPath();
        arrowLine.applyTransform (juce::AffineTransform::rotation (static_cast<float> (direction) * juce::MathConstants<float>::halfPi, pivot.getX(), pivot.getY()));
        g.setColour (linearSliderThumbTriFill.withAlpha (1.0f));
        g.strokePath (arrowLine, {3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded});
    }
    
    void drawThumbLinearCircle (juce::Graphics& g, float x, float y, float diameter, juce::Colour& colour) noexcept
    {
        /** Draw a shadow round.*/
        juce::Point<float> pivot { x, y };
        g.setColour (linearSliderThumbOuterRimColour);
        g.fillEllipse (juce::Rectangle<float> {diameter * 1.2f, diameter * 1.2f}.withCentre (pivot));
        /** Fill a central round. Gradient or not? */
        g.setColour (colour);
        g.fillEllipse (juce::Rectangle<float> {diameter * 0.5f, diameter * 0.5f}.withCentre (pivot));
        
        /** Draw an outer rim. */
        g.setColour (juce::Colours::orange);
        g.drawEllipse(juce::Rectangle<float> {diameter * 0.85f, diameter * 0.85f}.withCentre (pivot), 2.0f);
    }
 
    
    void drawThumbRotary () noexcept
    {
        
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SymmetricalLinearLookAndFeel)
    
};




