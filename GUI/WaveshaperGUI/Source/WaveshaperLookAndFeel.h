/*
  ==============================================================================

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

class WaveshaperLookAndFeel : public juce::LookAndFeel_V4
{
public:
    WaveshaperLookAndFeel()
    {}
    virtual ~WaveshaperLookAndFeel() override
    {}
    
    enum class OutlineType {ellipse, arcNormal, arcWithArrows, arcWithCornersOut, arcWithCornersIn, arcThreePointerEmpty, arcThreePointerFilled, noOutline};

    /** Slider functions. */
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
    
    void drawLinearSlider (juce::Graphics &, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle, juce::Slider &) override;
    
//    void drawLinearSliderBackground
    int getSliderThumbRadius (juce::Slider& slider) override;
    
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


private:
    const float edge {5.0f};
    const float sliderOuterRimScaleFactor {0.92f};
    bool outlineVisible {true};
    OutlineType localOutlineType {OutlineType::arcNormal};
    
    /**  Draw Rotary Slider thumb.
         Make it protected? a virtual? or make it helper, unless I want to use
         different thumbs in derived looks
     */
    void drawRotaryThumb (juce::Graphics& g, const juce::Point<float> centre, const float& radius, const float& angle)
    {
        /** Thumb dimentions (rectangle). */
        const float thumbWidth = radius * 0.27f;
        const float thumbHeight = radius * 0.33f;
        
        juce::Path p;
        p.addRectangle (-thumbWidth * 0.5, -radius * 0.87f, thumbWidth, thumbHeight);
        p.applyTransform (juce::AffineTransform::rotation (angle).translated (centre));
        g.fillPath (p);
    }
    
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
    return juce::jmin (12, slider.isHorizontal() ? static_cast<int> ((float) slider.getHeight() * 0.5f)
                 : static_cast<int> ((float) slider.getWidth()  * 0.5f));
}

inline void WaveshaperLookAndFeel::drawLinearSlider (juce::Graphics &, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, const juce::Slider::SliderStyle, juce::Slider &)
{
    
}

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
    
    /**  Draw Rotary Slider thumb. */
    void drawRotaryThumb (juce::Graphics& g, const juce::Point<float> centre, const float& radius, const float& angle)
    {
        /** Thumb dimentions (rectangle). */
        const float thumbWidth = radius * 0.27f;
        const float thumbHeight = radius * 0.33f;
        
        juce::Path p;
        p.addRectangle (-thumbWidth * 0.5, -radius * 0.87f, thumbWidth, thumbHeight);
        p.applyTransform (juce::AffineTransform::rotation (angle).translated (centre));
        g.fillPath (p);
    }
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SymmetricalRotaryLookAndFeel)
};


/** Custom L+F for the big and the small rotary slider. */

class RotaryBigLookAndFeel : public WaveshaperLookAndFeel
{
    
};




