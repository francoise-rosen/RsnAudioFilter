/*
  ==============================================================================

    SliderFrameSym.h
    Created: 14 Sep 2020 9:05:41am
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DelaySection : public Component
{
public:
    SliderFrameSym()
    {
        addAndMakeVisible(leftDelay.get());
        addAndMakeVisible(rightDelay.get());
        addAndMakeVisible(feedback.get());
    }
    
    ~SliderFrameSym() override
    {
        
    }
    
    void paint(juce::Graphics& g) override
    {
        
    }
    
    void resized()
    {
        
    }
    
    
private:
    std::unique_ptr<juce::Slider> leftDelay;
    std::unique_ptr<juce::Slider> rightDelay;
    std::unique_ptr<juce::Slider> feedback;
    
};
