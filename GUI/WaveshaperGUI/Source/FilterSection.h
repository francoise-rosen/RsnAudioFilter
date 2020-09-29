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
    FilterSection();
    ~FilterSection() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterSection)
};
