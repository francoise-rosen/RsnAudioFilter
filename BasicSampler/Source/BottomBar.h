/*
  ==============================================================================

    BottomBar.h
    Created: 30 Sep 2020 9:52:20am
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class BottomBar  : public juce::Component
{
public:
    BottomBar(BasicSamplerAudioProcessor& audioProcessor)
    :processor {audioProcessor}
    {

    }

    ~BottomBar() override
    {
    }

    void paint (juce::Graphics& g) override
    {

    }

    void resized() override
    {


    }

private:
    /** ADSR sliders */
    BasicSamplerAudioProcessor& processor;
    std::vector<std::unique_ptr<juce::Slider>> adsrSliders;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BottomBar)
};
