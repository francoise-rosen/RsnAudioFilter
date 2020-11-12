/*
  ==============================================================================

    FilterSection.h
    Created: 20 Sep 2020 12:27:10pm
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "WaveshaperLookAndFeel.h"

//==============================================================================
/*
*/
struct FilterControls
{
    juce::Slider frequency {juce::Slider::SliderStyle::Rotary, juce::Slider::TextEntryBoxPosition::TextBoxBelow};
    juce::Slider q {juce::Slider::SliderStyle::Rotary, juce::Slider::TextEntryBoxPosition::TextBoxBelow};
    juce::Slider boost {juce::Slider::SliderStyle::Rotary, juce::Slider::TextEntryBoxPosition::TextBoxBelow};
    juce::Slider amount {juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextEntryBoxPosition::TextBoxBelow};
    juce::ComboBox algorithm;
};

class FilterSection  : public juce::Component
{
public:
    FilterSection(juce::Colour parentBackground);
    virtual ~FilterSection() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void intialiseFilterControls (FilterControls& filterControls);

private:
    const float edge {5.0f};
    juce::Colour localBackground;
    WaveshaperLookAndFeel waveshaperLookAndFeel;
    FilterControls inputFilter;
    FilterControls outputFilter;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterSection)
};
