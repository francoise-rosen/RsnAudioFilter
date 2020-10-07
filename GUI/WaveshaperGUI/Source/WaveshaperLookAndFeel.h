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

    /** Slider functions. */
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
    
    void drawLinearSlider (juce::Graphics &, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle, juce::Slider &) override;
    
//    void drawLinearSliderBackground
//    void drawLinearSliderThumb

private:
    const float edge {5.0f};
    const float sliderOuterRimScaleFactor {0.92f};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveshaperLookAndFeel)
};

/** Slider functions.
    This is a middle rotary slider with outer rim, inner body, outer body, and a thumb.
    This is a plane version, so the inner body is placed right in the centre of the
    outer body.
 */
inline void WaveshaperLookAndFeel::drawRotarySlider (juce::Graphics &g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider &slider)
{
    auto innerBodyColour = slider.findColour (juce::Slider::rotarySliderFillColourId);
    auto outerBodyColour = slider.findColour (juce::Slider::rotarySliderOutlineColourId);
    juce::Point<float> centre {x + width * 0.5f, y + height * 0.5f};
    auto area = juce::Rectangle<int> (x, y, width, height).toFloat().reduced (edge);
    
    /** Outer body. */
    const float outerRadius = juce::jmin (width * 0.5f, height * 0.5f) * sliderOuterRimScaleFactor;
    juce::Point<float> outerRimXY {centre.getX() - outerRadius, centre.getY() - outerRadius};
    
    /** Inner body. */
    const float innerRadius = outerRadius * 0.9f;
    juce::Point<float> innerRimXY {centre.getX() - innerRadius, centre.getY() - innerRadius};
    
    g.setColour (outerBodyColour);
    g.drawEllipse(outerRimXY.getX(), outerRimXY.getY(), outerRadius * 2, outerRadius * 2, 3.0f);
    
    g.setColour (innerBodyColour);
    g.fillEllipse(innerRimXY.getX(), innerRimXY.getY(), innerRadius * 2.0f, innerRadius * 2.0f);
    

}

inline void WaveshaperLookAndFeel::drawLinearSlider (juce::Graphics &, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle, juce::Slider &)
{
    
}


/** Custom L+F for the big and the small rotary slider. */




