#pragma once

#include <JuceHeader.h>
#include "RosenSliderLookAndFeel.h"
#include "AlphaOneLookAndFeel.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    juce::Slider rotaryVerticalDrag {juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox};
    juce::Slider rotaryHorizontalDrag {juce::Slider::SliderStyle::RotaryHorizontalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox};
    juce::Slider rotaryVerticalHorizontalDrag {juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox};
    
    /** Linear. */
    juce::Slider linearV {juce::Slider::SliderStyle::LinearVertical, juce::Slider::TextEntryBoxPosition::TextBoxBelow};
    juce::Slider linearH {juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextEntryBoxPosition::TextBoxBelow};
    juce::Slider linearBar {juce::Slider::SliderStyle::LinearBar, juce::Slider::TextEntryBoxPosition::TextBoxBelow};
    juce::Slider linearBarV {juce::Slider::SliderStyle::LinearBarVertical, juce::Slider::TextEntryBoxPosition::TextBoxBelow};
    
    /** Two / Thee value sliders. */
    juce::Slider twoValHorizontal {juce::Slider::SliderStyle::TwoValueHorizontal, juce::Slider::TextEntryBoxPosition::TextBoxRight};
    juce::Slider twoValVertical {juce::Slider::SliderStyle::TwoValueVertical, juce::Slider::TextEntryBoxPosition::TextBoxBelow};
    juce::Slider threeValHorizontal {juce::Slider::SliderStyle::ThreeValueHorizontal, juce::Slider::TextEntryBoxPosition::TextBoxBelow};
    juce::Slider threeValVertical {juce::Slider::SliderStyle::ThreeValueVertical, juce::Slider::TextEntryBoxPosition::NoTextBox};

    RosenSliderLookAndFeel rosenLookAndFeel;
    AlphaOneLookAndFeel alphaLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
