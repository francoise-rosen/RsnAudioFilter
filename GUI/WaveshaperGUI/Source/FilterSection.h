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
class FilterSection  : public juce::Component
{
public:
    FilterSection(juce::Colour parentBackground);
    ~FilterSection() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    const float edge {5.0f};
    juce::Colour localBackground;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterSection)
};
