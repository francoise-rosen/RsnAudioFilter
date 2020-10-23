/*
  ==============================================================================

    LinearHorizontal.h
    Created: 21 Oct 2020 4:56:36pm
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once
#include <vector>
#include <JuceHeader.h>
#include "AlphaOneLookAndFeel.h"

class LinearHorizontal : public juce::Component
{
public:
    LinearHorizontal();
    ~LinearHorizontal() override;
    
    void paint (juce::Graphics& g) override;
    void resized () override;
    
    
private:
    enum SliderLF { basic=0, sym, symGradient, numSliders };
    std::vector<std::unique_ptr<juce::Slider>> sliders;
    void initialiseSliders ();
    
    AlphaOneLookAndFeel alphaOneLF;
    AlphaOneSymmetricalSlider alphaOneSymLF;
    AlphaOneSymmetricalSlider_V2 alphaOneSymGradLF;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LinearHorizontal)
    
    
};
