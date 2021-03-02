/*
  ==============================================================================

    LFOSection.h
    Created: 20 Sep 2020 12:27:44pm
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "WaveshaperLookAndFeel.h"

//==============================================================================
/*
*/
class LFOSection  : public juce::Component
{
public:
    LFOSection();
    virtual ~LFOSection() override;
    
    struct SendControl
    {
        //SendControl (const SendControl&);
        std::unique_ptr<juce::ToggleButton> toggle;
        std::unique_ptr<juce::Slider> amount;
        std::unique_ptr<juce::ComboBox> destination;
        // reserved area for the object contained in SendControl object
        std::unique_ptr<juce::Rectangle<int>> area_a;
    };

    void makeControls();

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider lfoRateSlider { juce::Slider::SliderStyle::Rotary, juce::Slider::TextEntryBoxPosition::TextBoxBelow };
    juce::Label label_A;
    const int numSends { 4 };
    std::vector<SendControl> controls;
    juce::ToggleButton toggle_a;
    juce::ComboBox box_a;
    juce::Slider destinationSlider_a { juce::Slider::SliderStyle::Rotary, juce::Slider::TextEntryBoxPosition::TextBoxBelow };
    WaveshaperLookAndFeel waveshaperLookAndFeel;
    
    /** areas. */
    std::unique_ptr<juce::Rectangle<int>> rateArea;
    std::unique_ptr<juce::Rectangle<int>> phaseArea;
    std::unique_ptr<juce::Rectangle<int>> sendArea;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOSection)
};
