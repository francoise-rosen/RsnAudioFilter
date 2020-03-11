/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.
 
 TO DO:
 1. initialise processor parameters
 2. add symmetry to all functions
 3. place gui
 4. test
 
 5. make Waveshaper class template (float and double)
 6. add cpu usage display

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class RaveshaperAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    RaveshaperAudioProcessorEditor (RaveshaperAudioProcessor&);
    ~RaveshaperAudioProcessorEditor();
    //==============================================================================
    void fillCombos();
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    RaveshaperAudioProcessor& processor;
    
    static std::vector<String> functions;
    
    Slider gain {Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow};
    Slider saturate {Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow};
    Slider symmetry {Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow};
    Slider crossfade {Slider::LinearHorizontal, Slider::TextBoxBelow};
    
    
    //Slider expCrossfade
    
    ComboBox function_A;
    ComboBox function_B;
    
    // TreeState
    //AudioProcessorValueTreeState::SliderAttachment gainAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> saturationAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> symmentryAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> crossfadeAttachment;
    
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> functionA_attachment;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> functionB_attachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RaveshaperAudioProcessorEditor)
};
