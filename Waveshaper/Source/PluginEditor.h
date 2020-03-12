/*
  ==============================================================================

  Based on this tutorial
  https://www.youtube.com/watch?v=7JUvVnRZrjg
 
 Parameters and GUI:
 One modulator

 
 GUI:
 1 Index of Modulation Dial (0, 1)
 2 Output volume dial (-.., 0.0dB)
 3 Symmetry dial
 4 Radio Button shape A
 5 Radio Button shape B
 6 Slider (horisontal) shape A - shape B
 
 

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
