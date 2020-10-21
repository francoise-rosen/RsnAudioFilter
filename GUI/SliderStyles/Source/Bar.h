/*
  ==============================================================================

    Bar.h
    Created: 21 Oct 2020 5:04:14pm
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class Bar  : public juce::Component
{
public:
    Bar();
    ~Bar() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider horizontalBar;
    juce::Slider horizontalBarGradient;
    juce::Slider verticalBar;
    juce::Slider verticalBarGradient;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Bar)
};
