/*
  ==============================================================================

    WaveformThumbnail.h
    Created: 6 Sep 2020 11:49:57am
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
   Draw a waveform using this class.
   Add time position marker.
   Make another version of it using AudioThumbnail class.
*/
class WaveformThumbnail  : public juce::Component
{
public:
    WaveformThumbnail();
    ~WaveformThumbnail() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformThumbnail)
};
