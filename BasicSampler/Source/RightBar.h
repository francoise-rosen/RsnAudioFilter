/*
  ==============================================================================

    RightBar.h
    Created: 30 Sep 2020 9:52:37am
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
   This is where LFO, amp detector, and saturator are placed
*/
class RightBar  : public juce::Component
{
public:
    RightBar()
    {
    
    }

    ~RightBar() override
    {
    }

    void paint (juce::Graphics& g) override
    {
    
    }

    void resized() override
    {

    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RightBar)
};
