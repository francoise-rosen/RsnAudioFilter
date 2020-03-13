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
 7 Dry / Wet (for instance wet can be bandlimited with HPF and LPF, whereas Dry still will keep LF components untouched)
 8 TransferA on/off (if we don't want to change saturation level or dry/wet, but want to move gradually from saturated state, we can turn off either of Transfer functions, like Octatrac slide)
 9 TransferB on/off
 
 

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class WaveshaperAudioProcessorEditor  : public AudioProcessorEditor

{
public:
    WaveshaperAudioProcessorEditor (WaveshaperAudioProcessor&);
    ~WaveshaperAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    

private:
 
    Slider gainSlider;
    Label dBLabel;

    double level = 0.0;
    
    // waveshaper gui
    Slider saturationSlider;
    
    WaveshaperAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveshaperAudioProcessorEditor)
    
    // connection between slider and parameter in tree state
    // if this pointer deleted, the connection brakes
    // don't delete the slider object or AudioProcessorTreeState object before this one!
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> saturationSliderAttachment;
};
