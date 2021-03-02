/*
  ==============================================================================

    RosenAlphaOneLookAndFeel
    WaveshaperLookAndFeel.h
    Created: 20 Sep 2020 12:29:48pm
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once
#include "Arrow.h"

/** Error when h + cpp, juce not found or expected class name error. */

/** Rotrary slider types:
    1. Big slider (default slider + inner and outer rim, perhaps different thumb)
    2. Default slider implemented in WaveshaperLookAndFeel - 3 coulours
    3. Small slider - 2 colours
 
 
 TO DO:
 CRITICAL: Popup (combobox) throws a leak error upon closing the window! It appears to be the problem with CrossfadeSection's comboboxes' PopupMenu. Solved by rewriting the comboboxes part of the code (simpified)
 - draw ComboBox, fix
 - draw TextButton On/OFF
 - draw + / - labels - done
 - draw labels for gui (names)
 - draw custom labels for gui (text boxes for linear sliders and gain sliders
 - draw arrows
 - add all the params
 - define default Font
 
 Colours:
 - set default colours for Components
 - set default colours for L+F
 - getters and setters for L+F colours
 - ColourScheme ?
 
 - make a plugin
 
 Git:
 - make different branches for trying colour palette / default colours
 */

//================================================================================

class WaveshaperLookAndFeel : public juce::LookAndFeel_V4
{
public:
    WaveshaperLookAndFeel()
    {
        /** Slider default colours */
        setColour (juce::Slider::backgroundColourId, juce::Colours::darkcyan);
        setColour (juce::Slider::trackColourId, juce::Colours::cyan);
        setColour (juce::Slider::thumbColourId, juce::Colours::darkcyan);
        setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::white);
        setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colours::white);
        setColour (juce::Slider::textBoxTextColourId, juce::Colours::silver.withBrightness (0.75f));
        setColour (juce::Slider::textBoxBackgroundColourId, juce::Colours::darkcyan.withBrightness (0.5f));
        setColour (juce::Slider::textBoxOutlineColourId, juce::Colours::red.withBrightness (0.2f));
    
        setColour (juce::ComboBox::backgroundColourId, juce::Colours::black);
        setColour (juce::ComboBox::outlineColourId, juce::Colours::white.withAlpha (0.75f));
        setColour (juce::PopupMenu::backgroundColourId, juce::Colours::black.withAlpha (0.5f));
        
        /** Label. */
        setColour (juce::Label::backgroundColourId, juce::Colours::darkblue);
        setColour (juce::Label::textColourId, juce::Colours::silver.withBrightness (0.5f));
        setColour (juce::Label::outlineColourId, juce::Colours::black.withAlpha(0.2f));
        setColour (juce::Label::backgroundWhenEditingColourId, juce::Colours::blue.withAlpha (0.5f));
        
        /** Button. */
        setColour (juce::TextButton::buttonColourId, juce::Colours::silver.darker());
        setColour (juce::TextButton::buttonOnColourId, juce::Colours::orange);
        
        
    }
    virtual ~WaveshaperLookAndFeel() override
    {}
    
    enum class OutlineType {ellipse, arcNormal, arcWithArrows, arcWithCornersOut, arcWithCornersIn, arcThreePointerEmpty, arcThreePointerFilled, noOutline};
    enum class ButtonShape { RoundedRect, Rect, Circle, Custom };

    /** Slider functions. */
    //================================================================================
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
    
    int getSliderThumbRadius (juce::Slider& slider) override;
    void drawLinearSlider (juce::Graphics &, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle, juce::Slider &) override;
    juce::Label* createSliderTextBox (juce::Slider& slider) override;
    juce::Slider::SliderLayout getSliderLayout (juce::Slider& slider) override;
    
//    void drawLinearSliderBackground
    void setThumbGradientTargetColour (const juce::Colour& colour);
    void setTrackGradientTargetColour (const juce::Colour& colour);
    void setThumbShadowColour (const juce::Colour& colour);
    
    
    /** Test this (unit test).
        The drawing may overcomplicate the drawRotary method.
        Delegate drowing the arc to other methods?
     */
    void setOutlineType (OutlineType outlineType)
    {
        localOutlineType = outlineType;
    }
    
    void setThumbPosition (bool isOnTop)
    {
        isThumbOnTop = isOnTop;
    }
    
    /** If outline is not visible, only sliderFill and sliderThumb
     are visible, but outer body (outline is still drown, it's just transparent.
     */
    
    bool isOutlineVisible() const
    {
        return outlineVisible;
    }
    
    void setOutlineVisibility (bool isVisible)
    {
        outlineVisible = isVisible;
    }
    
    void setTrackVisibility (bool isVisible)
    {
        isTrackVisible = isVisible;
    }
    
    //================================================================================
    /** Buttons. */
    juce::Font getTextButtonFont (juce::TextButton& t, int buttonHeight) override;
    void drawButtonBackground (juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour, bool isButtonHighlighted, bool isButtonDown) override;
    
    //================================================================================
    /** ComboBox, PopupMenu etc. */
    void drawComboBox (juce::Graphics& g, int width, int height, bool down, int buttonX, int buttonY, int buttonW, int buttonH, juce::ComboBox& box) override;
    //juce::Font getComboBoxFont (juce::ComboBox& box) override;
    void drawPopupMenuBackground (juce::Graphics& g, int width, int height) override;
    void positionComboBoxText (juce::ComboBox& box, juce::Label& label) override;
    juce::Font getFont() const
    {
        return defaultFont;
    }
    juce::Font getComboBoxFont (juce::ComboBox& box) override;
    
    //================================================================================
    /** Labels */
    juce::Font getLabelFont (juce::Label& l) override;
    juce::BorderSize<int> getLabelBorderSize (juce::Label& l) override;
    void drawLabel (juce::Graphics& g, juce::Label& j) override;
    
protected:
    /**  Draw Rotary Slider thumb.
     Make it protected? a virtual? or make it helper, unless I want to use
     different thumbs in derived looks
     */
    virtual void drawRotaryThumb (juce::Graphics& g, const juce::Point<float> centre, const float& radius, const float& angle) noexcept
    {
        /** Thumb dimentions (rectangle). */
        const float thumbWidth = radius * 0.27f;
        const float thumbHeight = radius * 0.33f;
        
        juce::Path p;
        p.addRectangle (-thumbWidth * 0.5, -radius * 0.87f, thumbWidth, thumbHeight);
        p.applyTransform (juce::AffineTransform::rotation (angle).translated (centre));
        g.fillPath (p);
    }
    
    // set default font here
    float fontHeight {14.0f};
    juce::Font defaultFont  {"Monaco", "Plain", fontHeight};
private:
    const float edge {5.0f};
    const float sliderOuterRimScaleFactor {0.92f};
    bool outlineVisible {true};
    bool isThumbOnTop {true};
    bool isTrackVisible {false};
    OutlineType localOutlineType {OutlineType::arcNormal};
    ButtonShape buttonShape { ButtonShape::Circle };
    
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
    //=========================================
    /** TEMP. */
    ColourScheme colourScheme = getMidnightColourScheme();
    g.setColour (colourScheme.getUIColour (ColourScheme::outline));
    //g.setColour (juce::Colours::yellow);
    //=========================================
    auto fill = slider.findColour (juce::Slider::rotarySliderFillColourId);
    auto outline = slider.findColour (juce::Slider::rotarySliderOutlineColourId);
    juce::Point<float> centre {x + width * 0.5f, y + height * 0.5f};
    auto area = juce::Rectangle<int> (x, y, width, height).toFloat().reduced (5.0f);
    //=========================================
    /** TEMP. */
    g.drawRect (area);
    //=========================================
    auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
    
    /** Outer body and outline. */
    const float outerRadius = juce::jmin (area.getWidth() * 0.5f, area.getHeight() * 0.5f) * sliderOuterRimScaleFactor;
     //const float outerRadius = juce::jmin (area.getWidth() * 0.5f, area.getHeight() * 0.5f) - 5.0f;
    const float rimWidth {2.0f};
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
    
    if (isThumbOnTop)
    {
        g.setColour (fill);
        g.fillEllipse (innerRimXY.getX(), innerRimXY.getY(), innerRadius * 2.0f, innerRadius * 2.0f);
        g.setColour (slider.findColour (juce::Slider::thumbColourId));
        drawRotaryThumb (g, centre, outerRadius, angle);
    }
    else
    {
        g.setColour (slider.findColour (juce::Slider::thumbColourId));
        drawRotaryThumb (g, centre, outerRadius, angle);
        g.setColour (fill);
        g.fillEllipse (innerRimXY.getX(), innerRimXY.getY(), innerRadius * 2.0f, innerRadius * 2.0f);
    }
}

inline int WaveshaperLookAndFeel::getSliderThumbRadius (juce::Slider& slider)
{
    return juce::jmin (12, slider.isHorizontal() ? static_cast<int> ((float) slider.getHeight() * 0.75f)
                 : static_cast<int> ((float) slider.getWidth()  * 0.75f));
}

/** Thumb : Triangle, Arrow, Circle
    ThumbFill : OneColour, Gradient
    Track : OneColour, Gradient
 */
inline void WaveshaperLookAndFeel::drawLinearSlider (juce::Graphics &, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, const juce::Slider::SliderStyle, juce::Slider &)
{
    /** At first only Horizonal and Vertical Linear sliders. */
    
    
}

inline juce::Label* WaveshaperLookAndFeel::createSliderTextBox (juce::Slider& slider)
{
    auto* l = juce::LookAndFeel_V2::createSliderTextBox (slider);
    /** For linear slider / bar. */
    //l->setColour (juce::Label::outlineColourId, juce::Colours::red);
    /** The following does not work. */
//    auto textBoxPos = slider.getTextBoxPosition();
//    if (textBoxPos == juce::Slider::TextBoxBelow || textBoxPos == juce::Slider::TextBoxAbove)
//    {
//        juce::Font font = defaultFont.withHeight (juce::jmin (14.0f, slider.getHeight() * 0.1f));
//        l->setFont (font);
//    }
    
    /** User must be able to change these! */
//    auto colour = l->findColour (juce::Label::backgroundColourId).withMultipliedAlpha (0.1f);
//    l->setColour (juce::Label::backgroundColourId, slider.findColour (juce::Slider::thumbColourId).withMultipliedAlpha (0.5f));
//    l->setColour (juce::Label::outlineColourId, slider.findColour (juce::Slider::textBoxOutlineColourId));
    l->setColour (juce::Label::backgroundColourId, slider.findColour (juce::Slider::textBoxBackgroundColourId));
    l->setColour (juce::Label::outlineColourId, slider.findColour (juce::Slider::textBoxOutlineColourId));
    l->setColour (juce::Label::textColourId, slider.findColour (juce::Slider::textBoxTextColourId));
    l->setFont (defaultFont);
    l->setMinimumHorizontalScale (1.0f);
    return l;
}

inline juce::Slider::SliderLayout WaveshaperLookAndFeel::getSliderLayout (juce::Slider& slider)
{
    // 1. compute the actually visible textBox size from the slider textBox size and some additional constraints
    int minXSpace = 0;
    int minYSpace = 0;
    auto textBoxPos = slider.getTextBoxPosition();
    if (textBoxPos == juce::Slider::TextBoxLeft || textBoxPos == juce::Slider::TextBoxRight)
        minXSpace = 20;
    else
        minYSpace = 15;
    auto localBounds = slider.getLocalBounds();
    auto textBoxWidth  = juce::jmax (0, juce::jmin (slider.getTextBoxWidth(),  localBounds.getWidth() - minXSpace));
    auto textBoxHeight = juce::jmax (0, juce::jmin (slider.getTextBoxHeight(), localBounds.getHeight() - minYSpace));
    juce::Slider::SliderLayout layout;
    // 2. set the textBox bounds
    if (textBoxPos != juce::Slider::NoTextBox)
    {
        if (slider.isBar())
        {
            layout.textBoxBounds = localBounds;
        }
        else
        {
            layout.textBoxBounds.setWidth (textBoxWidth);
            layout.textBoxBounds.setHeight (textBoxHeight);
            if (textBoxPos == juce::Slider::TextBoxLeft)           layout.textBoxBounds.setX (0);
            else if (textBoxPos == juce::Slider::TextBoxRight)     layout.textBoxBounds.setX (localBounds.getWidth() - textBoxWidth);
            else /* above or below -> centre horizontally */ layout.textBoxBounds.setX ((localBounds.getWidth() - textBoxWidth) / 2);
            if (textBoxPos == juce::Slider::TextBoxAbove)          layout.textBoxBounds.setY (0);
            else if (textBoxPos == juce::Slider::TextBoxBelow)     layout.textBoxBounds.setY (localBounds.getHeight() - textBoxHeight);
            else /* left or right -> centre vertically */    layout.textBoxBounds.setY ((localBounds.getHeight() - textBoxHeight) / 2);
        }
    }
    // 3. set the slider bounds
    layout.sliderBounds = localBounds;
    if (slider.isBar())
    {
        layout.sliderBounds.reduce (1, 1);   // bar border
    }
    else
    {
        if (textBoxPos == juce::Slider::TextBoxLeft)       layout.sliderBounds.removeFromLeft (textBoxWidth);
        else if (textBoxPos == juce::Slider::TextBoxRight) layout.sliderBounds.removeFromRight (textBoxWidth);
        else if (textBoxPos == juce::Slider::TextBoxAbove) layout.sliderBounds.removeFromTop (textBoxHeight);
        else if (textBoxPos == juce::Slider::TextBoxBelow) layout.sliderBounds.removeFromBottom (textBoxHeight);
        const int thumbIndent = getSliderThumbRadius (slider);
        if (slider.isHorizontal())    layout.sliderBounds.reduce (thumbIndent, 0);
        else if (slider.isVertical()) layout.sliderBounds.reduce (0, thumbIndent);
    }
    return layout;
}

/** Button methods. */
inline juce::Font WaveshaperLookAndFeel::getTextButtonFont (juce::TextButton& tb, int height)
{
    return defaultFont.withHeight(juce::jmin (16.0f, tb.getHeight() * 0.5f));
}

inline void WaveshaperLookAndFeel::drawButtonBackground (juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour, bool isButtonHighlighted, bool isButtonDown)
{
    auto area = button.getLocalBounds().toFloat().reduced (1.0f);
    auto cornerSize = juce::jmin (6.0f, juce::jmin (area.getWidth(), area.getHeight()) * 0.25f);
    auto baseColour = backgroundColour.withMultipliedSaturation ((button.hasKeyboardFocus (true) ? 1.3f : 0.9f)).withMultipliedAlpha((button.isEnabled()) ? 1.0f : 0.5f);
    if (isButtonDown || isButtonHighlighted)
        baseColour = baseColour.contrasting (isButtonDown ? 0.2f : 0.05f);
    auto buttonOn = button.findColour (juce::TextButton::buttonOnColourId);
    const float maxRadius = juce::jmin (area.getWidth() * 0.5f, area.getHeight() * 0.5f);
    const float rimWidth { 2.0f };
    //const float shadowWidth { 4.0f };
    const float shadowWidth { juce::jmin (maxRadius * 0.25f, 4.0f) };
    if (buttonShape == ButtonShape::Circle)
    {
        const float rimRadius = maxRadius - rimWidth;
        const float shadowRadius = maxRadius - shadowWidth;
        juce::Point<float> rimXY {area.getCentreX() - maxRadius, area.getCentreY() - maxRadius};
        if (isButtonDown)
        {
            g.setColour (buttonOn);
            g.fillEllipse(rimXY.getX(), rimXY.getY(), maxRadius * 2.0f, maxRadius * 2.0f);
            g.setColour (juce::Colours::black.withMultipliedAlpha (0.5f));
            g.drawEllipse (rimXY.getX() + shadowWidth , rimXY.getY() + shadowWidth, shadowRadius * 2.0f, shadowRadius * 2.0f, shadowWidth);
        }
        else
        {
            g.setColour (baseColour);
            g.fillEllipse(rimXY.getX(), rimXY.getY(), maxRadius * 2.0f, maxRadius * 2.0f);
        }
        g.setColour (juce::Colours::black);
        g.drawEllipse (rimXY.getX() + rimWidth, rimXY.getY() + rimWidth , rimRadius * 2.0f, rimRadius * 2.0f, rimWidth);
    }
}



/** ComboBox methods. */

inline void WaveshaperLookAndFeel::drawComboBox (juce::Graphics& g, int width, int height, bool down, int buttonX, int buttonY, int buttonW, int buttonH, juce::ComboBox& box)
{
    /* box properties */
    auto cornerSize = box.findParentComponentOfClass<juce::ChoicePropertyComponent>() != nullptr ? 0.0f : juce::jmax (height * 0.1f, 3.0f);
    juce::Rectangle<int> area (0, 0, width, height);
    g.setColour (box.findColour (juce::ComboBox::backgroundColourId));
    g.fillRoundedRectangle (area.toFloat(), cornerSize);
    g.setColour (box.findColour (juce::ComboBox::outlineColourId));
    g.drawRoundedRectangle (area.toFloat(), cornerSize, 1.0f);
    //g.drawRect (area);
    
    /* an arrow */
//    float side = juce::jmin (area.getWidth() * 0.25f, area.getHeight() * 0.6f);
//    juce::Point<float> arrowCentre ;
    float side = juce::jmin (area.getWidth() * 0.1f, area.getHeight() * 0.2f);
    juce::Rectangle<float> arrowBox {(juce::Rectangle<float>{side, side}).withCentre ({width - edge - side * 0.5f, (float)area.getCentreY()})};
    juce::Path p;
    p.startNewSubPath (arrowBox.getX(), arrowBox.getY());
    p.lineTo (arrowBox.getCentreX(), arrowBox.getBottom());
    p.lineTo (arrowBox.getRight(), arrowBox.getY());
    p.closeSubPath();
    g.strokePath (p, juce::PathStrokeType {2.0f});
    //g.drawRect((juce::Rectangle<float>{side, side}).withCentre (arrowCentre));
    
}

inline juce::Font WaveshaperLookAndFeel::getComboBoxFont (juce::ComboBox& box)
{
    return defaultFont.withHeight (juce::jmin (fontHeight, box.getHeight() * 0.72f));
}

inline void WaveshaperLookAndFeel::positionComboBoxText (juce::ComboBox& box, juce::Label& label)
{
    label.setColour (juce::Label::outlineColourId, box.findColour (juce::ComboBox::backgroundColourId).withMultipliedAlpha(0.0f));
    label.setBounds (1, 1,
                     box.getWidth() - 10,
                     box.getHeight() - 2);
    label.setFont (getComboBoxFont (box));
}

/** Label. */
inline juce::Font WaveshaperLookAndFeel::getLabelFont (juce::Label& l)
{
    return l.getFont();
}

inline juce::BorderSize<int> WaveshaperLookAndFeel::getLabelBorderSize(juce::Label& l)
{
    return l.getBorderSize();
}

inline void WaveshaperLookAndFeel::drawLabel (juce::Graphics& g, juce::Label& label)
{
    // fill the area
    g.fillAll (label.findColour (juce::Label::backgroundColourId));

    // set the text properties
    if (! label.isBeingEdited())
    {
        auto alpha = (label.isEnabled()) ? 1.0f: 0.2f;
        const juce::Font font {getLabelFont (label)};
        g.setColour (label.findColour (juce::Label::textColourId).withMultipliedAlpha (alpha));
        g.setFont (font);
        /** Draw the text. */
        auto textArea = getLabelBorderSize (label).subtractedFrom     (label.getLocalBounds());
        g.drawFittedText (label.getText(), textArea, label.getJustificationType(), juce::jmax (1, static_cast<int> (static_cast<float>(textArea.getHeight()) / font.getHeight())), label.getMinimumHorizontalScale() );
        g.setColour (label.findColour (juce::Label::outlineColourId).withMultipliedAlpha (alpha));
    }
    else if (label.isEnabled())
    {
        g.setColour (label.findColour (juce::Label::outlineColourId));
    }

    // draw a rounded rectangle
    //g.setColour (label.findColour (juce::Label::outlineColourId));
    g.drawRect (label.getLocalBounds());
}

inline void WaveshaperLookAndFeel::drawPopupMenuBackground (juce::Graphics& g, int width, int height)
{
    g.fillAll (findColour (juce::PopupMenu::backgroundColourId));
    
#if ! JUCE_MAC
    g.setColour (findColour (PopupMenu::textColourId).withAlpha (0.6f));
    g.drawRect (0, 0, width, height);
#endif
}

//================================================================================
/** Custom L+F for symmetrical knob. */

class SymmetricalRotaryLookAndFeel : public WaveshaperLookAndFeel
{
public:
    SymmetricalRotaryLookAndFeel()
    {
        setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colours::white);
    }
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
        const float innerRadius = outerRadius * knobToArcScaleFactor;
        juce::Point<float> innerRimXY {centre.getX() - innerRadius, centre.getY() - innerRadius};
        
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
        juce::Path middleLine;
        middleLine.startNewSubPath (centre.getX(), centre.getY() - outerRadius);
        middleLine.lineTo (centre.getX(), y);
        g.setColour (outline);
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
    float knobToArcScaleFactor {0.69f};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SymmetricalRotaryLookAndFeel)
};


//================================================================================
/** Custom L+F for the big rotary slider. */

class RotarySliderBigLookAndFeel : public WaveshaperLookAndFeel
{
public:
    RotarySliderBigLookAndFeel()
    {
        setColour (juce::Slider::backgroundColourId, juce::Colours::blue.withBrightness (0.2f));
        setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::blue.withBrightness (0.2f));
        setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colours::darkcyan);
    }
    virtual ~RotarySliderBigLookAndFeel() override
    {}
    
    enum class RotaryBackgroundFillShape
    {
        Circular,
        Rectangular,
        Ellipse
    };
    
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle,
                           float rotaryEndAngle, juce::Slider& slider) override
    {
        
        auto background = slider.findColour (juce::Slider::backgroundColourId);
        auto fill = slider.findColour (juce::Slider::rotarySliderFillColourId);
        auto outline = slider.findColour (juce::Slider::rotarySliderOutlineColourId);
        auto thumb = slider.findColour (juce::Slider::thumbColourId);
        juce::Point<float> centre {x + width * 0.5f, y + height * 0.5f};
        auto area = juce::Rectangle<int> (x, y, width, height).toFloat().reduced (edge);
        auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
        
        /** Knob area and slider area test drawing. */
        g.setColour (juce::Colours::orange);
        g.drawRect (area);
        g.setColour (slider.findColour (juce::Slider::backgroundColourId));
        g.drawRect (slider.getLocalBounds());
        /** End of the test. */
        
        /** Outer body. */
        float outerRimArcWidth {3.0f};
        float outerRimRadius {juce::jmin (area.getWidth() * 0.5f, area.getHeight() * 0.5f) - ((outerRimArcWidth > edge - 1.0f) ? outerRimArcWidth - edge - 1.0f : 0.0f)};
        juce::Point<float> outerRimXY {centre.getX() - outerRimRadius, centre.getY() - outerRimRadius};
        
        g.setColour (background);
        g.drawRect (juce::Rectangle<int> (x, y, width, height));
        if (backgroundFill == RotaryBackgroundFillShape::Circular)
        {
            g.fillEllipse (outerRimXY.getX(), outerRimXY.getY(), outerRimRadius * 2.0f, outerRimRadius * 2.0f);
        }
        else if (backgroundFill == RotaryBackgroundFillShape::Rectangular)
        {
            g.fillRect (area);
        }
        
        /** Arc section. */
//        float arcRadius = outerRimRadius - ( (edge > outerRimArcWidth) ? edge : outerRimArcWidth + edge);
        float arcRadius = outerRimRadius * 0.92f;
        juce::Point<float> arcXY { centre.getX() - arcRadius, centre.getY() - arcRadius };
        juce::Path arc;
        arc.addCentredArc (centre.getX(),
                           centre.getY(),
                           arcRadius,
                           arcRadius,
                           0.0f,
                           rotaryStartAngle,
                           rotaryEndAngle,
                           true);
        g.setColour (outline);
        g.strokePath (arc, { outerRimArcWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });
        /** The thumb. */
        g.setColour (thumb);
        drawRotaryThumb(g, centre, arcRadius, angle);
        
        /** The knob. */
        const float knobRadius = arcRadius * knobToArcScaleFactor;
        juce::Point<float> knobXY { centre.getX() - knobRadius, centre.getY() - knobRadius };
        juce::ColourGradient gradient {
            fill,
            centre,
            thumb,
            centre.withX (centre.getX() - knobRadius),
            true
        };
        g.setGradientFill (gradient);
        g.fillEllipse (knobXY.getX(), knobXY.getY(), knobRadius * 2.0f, knobRadius * 2.0f);
    }
    
    void setRotaryBackgroundFeelShape (const RotaryBackgroundFillShape shape)
    {
        backgroundFill = shape;
    }
    
private:
    const float edge {5.0f};
    const float knobToArcScaleFactor {0.72f};
    RotaryBackgroundFillShape backgroundFill {RotaryBackgroundFillShape::Circular};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RotarySliderBigLookAndFeel)
    
};


//================================================================================
/** Custom L+F for the small rotary slider. */

class SmallRotaryLookAndFeel : public WaveshaperLookAndFeel
{
public:
    SmallRotaryLookAndFeel(const bool& b = false)
    : symmetric {b}
    {
        //setColour (juce::Slider::backgroundColourId, juce::Colours::silver.withAlpha (0.3f));
        //setColour (juce::Slider::backgroundColourId, juce::Colours::darkorange);
        setColour (juce::Slider::backgroundColourId, juce::Colours::darkgrey);
        setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::darkcyan.withBrightness (0.25f));
        setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colours::black);
        setColour (juce::Slider::thumbColourId, juce::Colours::black);
        setColour (juce::Slider::trackColourId, juce::Colours::blue.withBrightness(0.2f));
    }
    virtual ~SmallRotaryLookAndFeel()
    {}
    
    /** Update this to draw arc for the rim,
     instead of having to draw a shadow around the thumb point. */
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override
    {
        auto area = juce::Rectangle<int> (x, y, width, height).toFloat().reduced (5.0f);
        juce::Point<float> centre {area.getCentre()};
        auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
        auto outerRadius { juce::jmin (area.getWidth() * 0.5f, area.getHeight()) * 0.5f };
        juce::Point<float> outerRimXY {centre.getX() - outerRadius, centre.getY() - outerRadius};
        auto innerRadius {outerRadius * 0.95f};
        juce::Point<float> innerRimXY {centre.getX() - innerRadius, centre.getY() - innerRadius};
        /** This is the colour that will be more present closer
         to the outer rim. */
        auto fillColour = slider.findColour (juce::Slider::rotarySliderFillColourId);
        auto background = slider.findColour (juce::Slider::backgroundColourId);
        juce::ColourGradient gradient
        {
            background.withMultipliedAlpha(0.93f),
            centre,
            fillColour,
            outerRimXY,
            true
        };
        g.setColour (background);
        g.drawEllipse(outerRimXY.getX(), outerRimXY.getY(), outerRadius * 2.0f, outerRadius * 2.0f, juce::jmin (5.0f, outerRadius * 0.5f));
        
        if (symmetric)
        {
            const float length = juce::jmin (5.0f, outerRadius * 0.2f);
            juce::Path pointer;
            pointer.startNewSubPath (centre.withY (centre.getY() - outerRadius));
            pointer.lineTo (centre.withY (centre.getY() - outerRadius - length));
            g.strokePath (pointer, juce::PathStrokeType {3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded});
        }
        
        g.setGradientFill(gradient);
        g.fillEllipse (innerRimXY.getX(), innerRimXY.getY(), innerRadius * 2.0f, innerRadius * 2.0f);
        
        /** draw a track. */
        juce::Path track;
        
        if (symmetric)
        {
            float mid = (rotaryStartAngle + rotaryEndAngle) * 0.5f;
            track.addCentredArc(centre.getX(),
                               centre.getY(),
                               outerRadius + 1.0f,
                               outerRadius + 1.0f,
                               0.0f,
                               (mid > angle) ? angle : mid,
                               (angle > mid) ? angle : mid,
                               true);
        }
        else
        {
            track.addCentredArc(centre.getX(),
                                centre.getY(),
                                outerRadius + 1.0f,
                                outerRadius + 1.0f,
                                0.0f,
                                rotaryStartAngle,
                                angle,
                                true);
        }
        g.setColour (slider.findColour (juce::Slider::trackColourId));
        g.strokePath (track, juce::PathStrokeType { juce::jmin (4.0f, outerRadius * 0.4f), juce::PathStrokeType::curved, juce::PathStrokeType::rounded });
        
        g.setColour (slider.findColour (juce::Slider::rotarySliderOutlineColourId));
        g.drawEllipse (innerRimXY.getX(), innerRimXY.getY(), innerRadius * 2.0f, innerRadius * 2.0f, juce::jmin (2.0f, outerRadius * 0.25f));
        
        g.setColour (background.withAlpha(0.85f));
        drawThumbShadow (g, centre, innerRadius, juce::jmin (2.0f, outerRadius * 0.25f), angle);
        g.setColour (slider.findColour (juce::Slider::thumbColourId));
        drawSliderThumb(g, centre, innerRadius, angle);
    }
    bool isSymmetrical() const
    {
        return symmetric;
    }
    void setSymmetrical (bool b)
    {
        symmetric = b;
    }
private:
    bool symmetric;
    void drawThumbShadow (juce::Graphics& g, const juce::Point<float>& centre, const float& trackRadius, const float& radius, const float& angle)
    {
        juce::Point<float> thumbPoint { centre.getX() + trackRadius * std::cos (angle - juce::MathConstants<float>::halfPi), centre.getY() + trackRadius * std::sin (angle - juce::MathConstants<float>::halfPi) };
        g.fillEllipse (juce::Rectangle<float> (radius * 3.0f, radius * 3.0f).withCentre(thumbPoint));
    }
    
    void drawSliderThumb (juce::Graphics& g, juce::Point<float>& centre, const float& radius, const float& angle)
    {
        juce::Path p;
        p.startNewSubPath (centre.withY (centre.getY() - radius * 0.33f));
        p.lineTo (centre.withY (centre.getY() - radius));
        //p.applyTransform (juce::AffineTransform::rotation (angle).translated (centre));
        p.applyTransform (juce::AffineTransform::rotation (angle, centre.getX(), centre.getY()));
        g.strokePath (p, {2.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded});

    }
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SmallRotaryLookAndFeel)
};

//================================================================================
/** Custom L+F for the symmetrical linear slider. */
/** Define the default colours and shapes.
    Provide functions for user to customize those.
 Colours:
    - thumb colour (fills the round, triangle, arrow or rect) - default orange ?
    - background colour (fills the background) - default darkcyan ? blue.withBrightness (0.2f)
    - track colour - default cyan ? darkcyan?
 */
class SymmetricalLinearLookAndFeel : public WaveshaperLookAndFeel
{
public:
    SymmetricalLinearLookAndFeel()
    {}
    virtual ~SymmetricalLinearLookAndFeel() override
    {}
    
    enum class PointerFill { NoFill, Fill, FillGradient };
    enum class TriangleFillType { OneColour, Triangles, Pencil, Gradient };
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
    
    /** Set colours and flags. */
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
    PointerFill pointerFill { PointerFill::FillGradient };
    TriangleFillType triangleFillType { TriangleFillType::Triangles };
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
        if (triangleFillType == TriangleFillType::Triangles)
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
        else if (triangleFillType == TriangleFillType::Pencil)
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
 
        if (pointerFill == PointerFill::Fill)
        {
            g.setColour (colour);
            g.fillEllipse (juce::Rectangle<float> {diameter * 0.5f, diameter * 0.5f}.withCentre (pivot));

        }
        
        else if (pointerFill == PointerFill::FillGradient)
        {
            juce::ColourGradient gradient {
                juce::Colours::orange,
                pivot.getX(),
                pivot.getY(),
                juce::Colours::black,
                pivot.getX() + diameter * 0.5f,
                pivot.getY() + diameter * 0.5f,
                true
            };
            g.setGradientFill (gradient);
            g.fillEllipse (juce::Rectangle<float> {diameter * 0.5f, diameter * 0.5f}.withCentre (pivot));
        }
        
        else
        {
            g.setColour (colour);
            g.drawEllipse (juce::Rectangle<float> {diameter * 0.5f, diameter * 0.5f}.withCentre (pivot), juce::jmin (3.0f, diameter * 0.1f));
        }
        
        /** Draw an outer rim. */
        g.setColour (juce::Colours::orange);
        g.drawEllipse(juce::Rectangle<float> {diameter * 0.85f, diameter * 0.85f}.withCentre (pivot), 2.0f);
    }
 
    
    void drawThumbRotary () noexcept
    {
        
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SymmetricalLinearLookAndFeel)
    
};

/** Specific non-editable labels for indicators like +, -, letters etc. */
/** Colours:
    1. Label background - fillColour. Default - fully transparent.
    2. outlineColour - juce::Label::outlineColourId
    3. gradientColourId - default (Compoonent background)
    4. Text colour - juce::Slider::textColourId
 
 */
class IndicatorLookAndFeel : public juce::LookAndFeel_V4
{
public:
    IndicatorLookAndFeel()
    {}
    
    IndicatorLookAndFeel (juce::Colour componentBackground)
    :fillColour {componentBackground.withAlpha(0.0f)}, gradientOuterColour {componentBackground}
    {}
    
    virtual ~IndicatorLookAndFeel() override
    {}
    
    enum class IndicatorShape { Ellipse, Circle, RoundedRectangle, Rectangle, Triangle };
    void setGradientOuterColour (juce::Colour colour)
    {
        gradientOuterColour = colour;
    }
    void setGradientOn (bool isOn)
    {
        isGradientOn = isOn;
    }
    void setFillColour (juce::Colour componentBackground)
    {
        fillColour = componentBackground;
    }
    void drawLabel (juce::Graphics& g, juce::Label& label) override
    {
        auto innerColour = label.findColour (juce::Label::backgroundColourId);
        outlineColour = label.findColour (juce::Label::outlineColourId);
        auto labelArea = label.getLocalBounds();
        juce::Point<float> pivot { labelArea.getWidth() * 0.5f, labelArea.getHeight() * 0.5f };
        
        g.fillAll (fillColour);
        
        std::unique_ptr<juce::ColourGradient> gradient;
        if (isGradientOn)
        {
            juce::Point<float> outerPoint;
            if ( (shape == IndicatorShape::Ellipse) ||
                 (shape == IndicatorShape::RoundedRectangle) ||
                 (shape == IndicatorShape::Rectangle ) )
            {
                outerPoint = juce::Point<float> {
                    (labelArea.getWidth() > labelArea.getHeight()) ? 0.0f : labelArea.getWidth() * 0.5f, (labelArea.getWidth() > labelArea.getHeight()) ? labelArea.getHeight() * 0.5f : 0.0f
                };
            }
            else if (shape == IndicatorShape::Circle)
            {
                auto diameter = juce::jmin (labelArea.getWidth(), labelArea.getHeight());
                outerPoint = juce::Point<float> {
                    pivot.getX() - diameter * 0.5f, pivot.getY() - diameter * 0.5f
                };
            }
            gradient = std::make_unique<juce::ColourGradient>
                    (innerColour,
                     pivot,
                     gradientOuterColour,
                     outerPoint,
                     (shape == IndicatorShape::Ellipse) ? true : false
                     );
        }
        
        g.setColour (innerColour);
        if (shape == IndicatorShape::RoundedRectangle)
        {
            g.fillRoundedRectangle (labelArea.toFloat(), 5.0f);
        }
        else if (shape == IndicatorShape::Rectangle)
        {
            g.fillRect (labelArea);
        }
        else if (shape == IndicatorShape::Ellipse)
        {
            if (isGradientOn)
            {
                g.setGradientFill (*gradient);
            }
            g.fillEllipse (labelArea.toFloat());
            auto margin = juce::jmin (2.0f, juce::jmin (labelArea.getWidth(), labelArea.getHeight()) * 0.1f);
            g.setColour (outlineColour);
            g.drawEllipse (labelArea.reduced (margin).toFloat(), margin);
        }
        else if (shape == IndicatorShape::Triangle)
        {
            /** I need to know the direction and the pivot point.
             I need to know how to fit it in label bounds when rotated. */
            
        }
    
        g.setColour (textColour);
        auto textArea = getLabelBorderSize (label).subtractedFrom (labelArea);
        g.drawFittedText (label.getText(), textArea, label.getJustificationType(), 1, label.getMinimumHorizontalScale());
    }
    
private:
    IndicatorShape shape { IndicatorShape::Ellipse };
    bool isGradientOn { false };
    
    /** How do I set these? */
    juce::Colour fillColour { juce::Colours::orange.withAlpha(0.25f) };
    juce::Colour outlineColour { juce::Colours::black.withAlpha (0.0f) };
    juce::Colour gradientOuterColour { juce::Colours::black };
    juce::Colour textColour { juce::Colours::white };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IndicatorLookAndFeel);
    
};






