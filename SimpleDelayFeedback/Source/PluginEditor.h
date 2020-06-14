/*
  ==============================================================================

CircularBuffer
    1. Build simple stereo delay feedback. Switch Mono - Stereo (2 different delays for L and R)
    1.1 Ping Pong on OF
    1.2 Clip, Fold, and Wrap functions
    1. SEND or MIX (dry/wet) option
    2. Add biquad filter in feedback path
    3. Add biquad filter in the input
    4. Class CircularBuffer with functions .read(readPos, blockSize), .write(writePos, blockSize)
 
 
 PRIVATE, CREATIVE
    4. Modulator for Delay (L and R) - LFO
    5. Modulator for Feedback - LFO
    6. Tempo based delay. How do I get this kind of info from a host?
 7. Delay mode: Micro (1 - 128 samples) - Macro (in sec)
    8. Delay path through Comb and Allpass blocks
    9. Make it a tree-like - Input -> delay A/B -> delay A1/A2 B1/B2. Also apply panning (which algo?)
 
    GRAPHICS
 1. Layout: how many params?
 2. Labels or g.drawText?
 3. LookAndFeel for knobs and labels?
 4. Volume meter (implement an AudioDetector class from Pircle)
 5. Area for every gui object, w different colour

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SimpleDelayFeedbackAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    SimpleDelayFeedbackAudioProcessorEditor (SimpleDelayFeedbackAudioProcessor&);
    ~SimpleDelayFeedbackAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
    using Processor = SimpleDelayFeedbackAudioProcessor;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Processor& processor;
    
    Slider gainSlider;
    Slider delaySlider;
    Slider feedbackSlider;
    
    Label gainLabel;
    Label delayLabel;
    Label fbLabel;
    
    const float edgeThickness = 20;
    const int textboxWidth = 70;
    const int textboxHeight = 30;
    
    // these objects maintain connection between sliders and parameters in
    // an AudioProcessorValueTreeState
    std::unique_ptr<SliderAttachment> gainSliderAttachment;
    std::unique_ptr<SliderAttachment> delaySliderAttachment;
    std::unique_ptr<SliderAttachment> freedBackSliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleDelayFeedbackAudioProcessorEditor)
    
    
};
