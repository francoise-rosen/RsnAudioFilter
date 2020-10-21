/*
  ==============================================================================

    LinearHorizontal.h
    Created: 21 Oct 2020 4:56:36pm
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

class LinearHorizontal : public juce::Component
{
public:
    LinearHorizontal();
    ~LinearHorizontal() override;
    
    void paint (juce::Graphics& g) override;
    void resized () override;
    
private:
    juce::Slider alphaHorizontal;
    juce::Slider alphaHorizontalSym;
    juce::Slider alphaHorizontalSymGradient;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LinearHorizontal)
    
    
};
