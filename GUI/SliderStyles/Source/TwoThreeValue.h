/*
  ==============================================================================

    TwoThreeValue.h
    Created: 21 Oct 2020 5:05:10pm
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class TwoThreeValue  : public juce::Component
{
public:
    TwoThreeValue();
    ~TwoThreeValue() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TwoThreeValue)
};
