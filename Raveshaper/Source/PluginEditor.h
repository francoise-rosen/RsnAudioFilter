/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.
 
 PLAN:
 1. DRAW GUI (align later)
 2. IMLEMENT SYMMETRY (DC offset in, DCblock out (class?))
 3. IMPLEMENT plugin processor blocks (reading, saving, using parameters)
 4. Align the GUI
 5. Oversampling

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
    
    void fillCombos();
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    static std::vector<String> functions;
    
    RaveshaperAudioProcessor& processor;
    
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
