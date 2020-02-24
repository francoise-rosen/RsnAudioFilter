/*
  ==============================================================================

  Based on this tutorial
  https://www.youtube.com/watch?v=7JUvVnRZrjg
 
 Parameters and GUI:
 One modulator
 
 1. Waveshaper function (radiobutton)
 2. Index of modulation (1.0 - 9.0)
 3. MIX (dry / wet)
 4. Symmetry knob
 ------------------
 4. LPF / HPF cutoff dial, q dial (later)
 5. LPG dial (later)
 
 --------------------
 
 Two modulators:
 1. Crossfade horisontal slider
 2. the rest is how for 1 modulator
 
 

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "DecibelSlider.h"

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
    //Slider gainSlider;
    // works like slider but has 2 overriden functions
    //DecibelSlider gainSliderdB;
    Slider gainSlider;
    Label dBLabel;
    // local level, not sure if needed
    double level = 0.0;
    
    // waveshaper gui
    Slider index;
    
    GainSliderAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainSliderAudioProcessorEditor)
    
public:
    // connection between slider and parameter in tree state
    // if this pointer deleted, the connection brakes
    // don't delete the slider object or AudioProcessorTreeState object before this one!
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sliderAttachment;
};
