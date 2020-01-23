/*
  ==============================================================================

  https://www.youtube.com/watch?v=7JUvVnRZrjg

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class GainSliderAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    GainSliderAudioProcessorEditor (GainSliderAudioProcessor&);
    ~GainSliderAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GainSliderAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainSliderAudioProcessorEditor)
};
