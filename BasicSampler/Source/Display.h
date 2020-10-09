/*
  ==============================================================================

    Display.h
    Created: 30 Sep 2020 9:51:32am
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "WaveformThumbnail.h"

//==============================================================================
/**
   Place the waveform drawn by WaveformThumbnail.
   Display the name of the loaded file.
*/
class Display  : public juce::Component
{
public:
    Display()
    {

    }

    ~Display() override
    {
    }

    void paint (juce::Graphics& g) override
    {

    }

    void resized() override
    {

    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Display)
};
