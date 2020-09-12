/*
  ==============================================================================

    SliderMidLookAndFeel.h
    Created: 12 Sep 2020 9:47:59am
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

class SliderMidLookAndFeel : public juce::LookAndFeel_V4
{
public:
    SliderMidLookAndFeel()
    {
        setColour(juce::Slider::thumbColourId, colourArray[outerRimColour]);
        setColour(juce::Slider::textBoxOutlineColourId, colourArray[backgroundColour]);
        setColour(juce::ComboBox::outlineColourId, colourArray[backgroundColour].darker());
        setColour(juce::ComboBox::backgroundColourId, colourArray[objectColour]);
        setColour(juce::PopupMenu::backgroundColourId, colourArray[backgroundColour].withAlpha(0.4f));
    }
    
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override
    {
        // smaller distance from centre to the edge of the slider's area
        float radius = static_cast<float>(juce::jmin(width / 2, height / 2)) * objectScale;
        juce::Point<float> centre = {static_cast<float>(x + width / 2), static_cast<float>(y + height / 2)};
        juce::Point<float> ellipseXY {centre.getX() - radius, centre.getY() - radius};
        float rw = radius * 2.0;
        float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
        
        //Outer rim
        float xShift = juce::jmin(width, height) / 30.0f;
        float yShift = juce::jmin(width, height) / 30.0f;
        float centreShift = std::sqrt(xShift * xShift + yShift * yShift);
        float rimRadius = radius + centreShift + juce::jmin(xShift, yShift);
        float rimRW = rimRadius * 2;
        juce::Point<float> rimEllipseXY {centre.getX() - rimRadius, centre.getY() - rimRadius};
        // knob gradient
        juce::Colour outerColour = colourArray[outerRimColour];
        juce::Colour innerColour = colourArray[outerRimColour].withBrightness(0.40f);
        
        if (knobWithGradient)
        {
            g.setGradientFill({outerColour, rimEllipseXY.getX(), height / 2.0f, innerColour, width * 0.85f, centre.getY(), true});
        }
        else
        {
            g.setColour(colourArray[outerRimColour]);
        }
        
        g.fillEllipse(rimEllipseXY.getX(), rimEllipseXY.getY(), rimRW, rimRW);
        
        g.setColour(colourArray[objectColour].withBrightness(0.57f));
        ellipseXY = {ellipseXY.getX() + xShift, ellipseXY.getY() + yShift};
        g.fillEllipse(ellipseXY.getX(), ellipseXY.getY(), rw, rw);
        
        
        //g.drawEllipse(ellipsePoint.getY().translated(-3.0f), rw + 3.0f, rw + 3.0f);
        g.setColour(colourArray[outerRimColour].withHue(0.29f));
        juce::Point<float> shiftedCentre {centre.getX() + xShift, centre.getY() + yShift};
        drawSliderThumb(g, shiftedCentre, radius, angle);
        
                                          
    }
    
    void setBackgroundColour(juce::Colour background)
    {
        colourArray[backgroundColour] = background;
    }
    
    void isWithGradient(bool grad)
    {
        knobWithGradient = grad;
    }
    
    
private:
    enum colours {backgroundColour, objectColour, outerRimColour};
    std::vector<juce::Colour> colourArray {juce::Colours::white, juce::Colours::orange.withHue(0.89f).withBrightness(0.37f), juce::Colours::black};
    const float objectScale {0.5f};
    bool knobWithGradient {true};
    
    
    void drawSliderThumb(juce::Graphics& g, juce::Point<float>& centre, const float& radius, const float& angle)
    {
        juce::Path p;
        
        // shape
        float sliderThumbLength = radius * 0.57f;
        float sliderThumbWidth = sliderThumbLength * 0.1f;
        p.addRoundedRectangle(-sliderThumbWidth * 0.5f, -radius, sliderThumbWidth, sliderThumbLength, 3.0f, 3.0f, true, true, false, true);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(centre));
        
        g.fillPath(p);
    }
    
    
};
