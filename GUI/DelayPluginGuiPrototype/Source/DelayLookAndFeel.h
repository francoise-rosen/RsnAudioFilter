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
    {
//        setColour(juce::Label::backgroundColourId, juce::Colours::red);
//        setColour(juce::Label::backgroundWhenEditingColourId, juce::Colours::black);
        setColour(juce::Label::outlineColourId, juce::Colours::transparentWhite);
        setColour(juce::Slider::textBoxTextColourId, juce::Colours::orange);
       // setColour(juce::Label::textColourId, juce::Colours::black);
    }
    
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
        
        /* draw a pointer */
        g.setColour(colourPalette[rim]);
        drawSliderThumb(g, topPlaneCentre, radius, angle);
        
        g.setColour(juce::Colours::red);
//        g.drawRect(x, y, width, height);
        
    }
    
    juce::Label* createSliderTextBox(juce::Slider& slider) override
    {
        auto* l = juce::LookAndFeel_V2::createSliderTextBox (slider);

        if (getCurrentColourScheme() == LookAndFeel_V4::getGreyColourScheme() && (slider.getSliderStyle() == juce::Slider::LinearBar
                                                                                  || slider.getSliderStyle() == juce::Slider::LinearBarVertical))
        {
            l->setColour (juce::Label::textColourId, juce::Colours::black.withAlpha (0.7f));
        }
        l->setFont(juce::Font("Monaco", "Italic", 4.0f));

        return l;
    }

    juce::Font getLabelFont(juce::Label& label) override
    {
        return juce::Font("Monaco", "Plane", 14.0f);
    }
    
    void drawLabel(juce::Graphics& g, juce::Label& label) override
    {

        g.setColour(colourPalette[shadow]);
        auto area = label.getLocalBounds().reduced(label.getWidth()* 0.02f, 1.0f);
        g.fillRoundedRectangle(area.toFloat(), 3.0f);

        if (! label.isBeingEdited())
        {
            auto alpha = label.isEnabled() ? 1.0f : 0.05f;
            const juce::Font font (getLabelFont(label));
            g.setColour (label.findColour (juce::Label::textColourId).withMultipliedAlpha (alpha));
            g.setFont (font);
            auto textArea {label.getBorderSize().subtractedFrom(area)};
            g.drawFittedText(label.getText(), textArea, label.getJustificationType(), juce::jmax (1, static_cast<int>(textArea.getHeight() / font.getHeight())), label.getMinimumHorizontalScale());
            g.setColour (label.findColour (juce::Label::outlineColourId).withMultipliedAlpha (alpha));

        }

        else if (label.isEnabled())
        {
            setColour(juce::Label::outlineColourId, juce::Colours::pink);
            g.setColour (label.findColour (juce::Label::outlineColourId));
            setColour(juce::Label::outlineColourId, juce::Colours::yellow);
        }
        g.drawRoundedRectangle(area.toFloat(), 3.0f, 1.0f);
        g.setColour(colourPalette[background].withAlpha(0.0f));
        g.fillRoundedRectangle(area.toFloat(), 3.0f);

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
    
    void setObjectScale(float val)
    {
        objectScale = val;
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
    float objectScale {0.5f}; /* slider's top area relative to entire slider's area */
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
    
    juce::Font createTextBoxFont()
    {
        return juce::Font("Monaco", "Plane", 14.0f);
    }
    
};

// ComboBox LookAndFeel

class ComboLookAndFeel : public juce::LookAndFeel_V4
{
public:
    ComboLookAndFeel()
    {}
    
    virtual ~ComboLookAndFeel()
    {}
    
    // set combo box font
    juce::Font getComboBoxFont(juce::ComboBox& box) override
    {
        return currentComboBoxFont();
    }
    
    // set combo box popup font
    
    juce::Font getPopupMenuFont() override
    {
        return currentComboBoxFont();
    }
    
    void drawComboBox (juce::Graphics& g, int width, int height, bool down, int buttonX, int buttonY, int buttonW, int buttonH, juce::ComboBox& box) override
    {
        /* box properties */
        auto cornerSize = box.findParentComponentOfClass<juce::ChoicePropertyComponent>() != nullptr ? 0.0f : 3.0f;
        juce::Rectangle<int> area (0, 0, width, height);
        g.setColour (box.findColour(juce::ComboBox::backgroundColourId));
        g.fillRoundedRectangle (area.toFloat(), cornerSize);
        g.setColour (box.findColour (juce::ComboBox::outlineColourId));
        g.drawRoundedRectangle (area.reduced(1.5f).toFloat(), cornerSize, 1.0f);
        
        /* an arrow */
        juce::Path arrow;
        juce::Point<float> point1 {width * 0.85f, height * 0.4f};
        juce::Point<float> point3 {width * 0.95f, height * 0.4f};
        juce::Point<float> point2 {width * 0.9f, height * 0.6f};
        g.setColour (box.findColour (juce::ComboBox::arrowColourId).withAlpha ((box.isEnabled() ? 0.59f : 0.2f )));
        arrow.addTriangle (point1, point2, point3);
        g.fillPath (arrow);
        
    }

    
private:
    int fontHeight {12};
    
    juce::Font currentComboBoxFont()
    {
        return juce::Font("Monaco", "Plain", fontHeight);
    }
    
};

/* Button Look And Feel */

class ButtonLookAndFeel : public juce::LookAndFeel_V4
{
public:
    ButtonLookAndFeel()
    {}
    
    virtual ~ButtonLookAndFeel()
    {}
    
    void drawButtonBackground (juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour, bool isButtonHighlighted, bool isButtonDown) override
    {
      
        auto area = button.getLocalBounds().reduced(3.0f, 3.0f);
        auto cornerSize = juce::jmin (area.getWidth() * 0.15f, area.getHeight() * 0.15f);
        
        juce::Colour baseColour = backgroundColour.withMultipliedSaturation (button.hasKeyboardFocus (true) ? 1.3f : 0.95f)
                                                  .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.05f);
           
        if (isButtonHighlighted || isButtonDown)
        {
            baseColour = baseColour.contrasting(isButtonDown ? 0.9f : 0.05f);
            
        }

        
        /* add shadow */
        auto edge = juce::jmin (2.0f, juce::jmin (area.getWidth() * 0.2f, area.getHeight() * 0.2f));
        auto buttonArea = area;
        buttonArea.removeFromTop (edge);
        buttonArea.removeFromLeft (edge);
        g.setColour(backgroundColour.withMultipliedBrightness(0.12f));
        g.fillRoundedRectangle(buttonArea.toFloat(), cornerSize);
        
        auto offset = isButtonDown ? edge * 0.62f : edge;
        buttonArea.translate(offset, offset);
        
        
        g.setColour(baseColour);
        g.fillRoundedRectangle (buttonArea.toFloat(), cornerSize);
        g.setColour (baseColour.contrasting(0.97f));
        g.drawRoundedRectangle (buttonArea.toFloat(), cornerSize, 1.0f);
        
   
        
        
        
    }
    
};

// SLIDER look and feel (with inner and outer rim)

// Big Slider look and feel (line make noise big slider)
