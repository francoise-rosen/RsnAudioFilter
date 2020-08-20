/*
  ==============================================================================
 
 PROBLEMS:
 - repetative click on both channels! (something SimpleDelay patch does not have) - fixed (wrong write position)
 - clicks when playback stopped (add fade out!) (something SimpleDelay patch has) - fixed (see above)
 - unstable Stereo, Gain 0.0 FB 0.0 after some time may blow up - fixed (didn't update read position)
 - Ping Pong not working at all - fixed (but still need to look into this)
 - w/o interpolation 2% CPU unlike DubFx that uses 1%
 - sample-wise hits 19% CPU, regardless interpolation! fuck! >10% of it cased by write and read functions - fixed by choosing Release of the Build in Edit Scheme - now it's5%
 - ping pong not working again! - fixed but neet to look it up again, to optimise the code

CircularBuffer - for Public GIT
    1. Build simple stereo delay feedback. - done
    1.1 Ping Pong or STEREO - works fine (using Delay.h) and reversed channels in processBlock() - done
    1.2 Clip, Fold, and Wrap functions (use fold in feedback path, to prevent blowup, test in gen). Only for sample-wise read and write
    1.3 Switch Mono - Stereo (2 different delays for L and R)
    1. SEND or MIX (dry/wet) option - low priority
    2. Add biquad (or VCF?) filter in feedback path (whatch out for Q values, no peak filters!). Perhaps FIR? - private
 3. Butterworth BandPass 1-4 orders with: Upper Freq, Lower Freq, Order parameters (only these!)
    4. Class CircularBuffer with functions .read(readPos, blockSize), .write(writePos, blockSize) - done
    4.1 Class Delay - up to 1 sample delay (not block related). Check SC DelayN, DelayC etc. - Karpus Strong algo etc. (don't check implementation of Karpus Strong yet!) - done
    5. Wire-And-Ing (page 396) - for a class with sample-wise read and write - done
    6. Smooth delay controls - add OnePole Filter
    7. Fractional Delay - for a class only, for now just linear interpolation. - done
    7.1 Lagrange, Cubic, Spline interpolation?
    8. Pack the sliders in OwnedArray<Slider> rotarySliders and Lables in OwnedArray<Label> labels
 
 
 PRIVATE, CREATIVE
    1. Cascade of Biquads Filter in fb path - 2 filters with classical waveforms 1-10 order, but make each biquad drywet controlled (w/ modulation possibilities)
    4. Modulator for Delay (L and R) - LFO
    5. Modulator for Feedback - LFO
    6. Tempo based delay. How do I get this kind of info from a host?
 7. Delay mode: Micro (1 - 128 samples) - Macro (in sec)
    8. Delay path through Comb and Allpass blocks
    9. Make it a tree-like - Input -> delay A/B -> delay A1/A2 B1/B2. Also apply panning (which algo?)
    10. Explore and implement pitch shifiters and other attributes of dub echo!
 
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
class StereoDelayProcessorEditor  : public AudioProcessorEditor
{
public:
    StereoDelayProcessorEditor (StereoDelayProcessor& p);
    ~StereoDelayProcessorEditor();
    
    void fillTypeBox();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
    using ComboBoxAttachment = AudioProcessorValueTreeState::ComboBoxAttachment;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    StereoDelayProcessor& processor;
    
    Slider gainSlider;
    Slider delaySliderLeft;
    Slider delaySliderRight;
    Slider feedbackSlider;
    Slider dryWetSlider;
    ComboBox delayTypeBox;
    
    Label gainLabel;
    Label delayLabelLeft;
    Label delayLabelRight;
    Label fbLabel;
    Label typeLabel;
    Label dryWetLable;
    
    const float edgeThickness = 10;
    const int textboxWidth = 70;
    const int textboxHeight = 30;
    
    // these objects maintain connection between sliders and parameters in
    // an AudioProcessorValueTreeState
    std::unique_ptr<SliderAttachment> gainSliderAttachment;
    std::unique_ptr<SliderAttachment> delaySliderLeftAttachment;
    std::unique_ptr<SliderAttachment> delaySliderRightAttachment;
    std::unique_ptr<SliderAttachment> freedbackSliderAttachment;
    std::unique_ptr<SliderAttachment> dryWetSliderAttachment;
    std::unique_ptr<ComboBoxAttachment> typeAttachment;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StereoDelayProcessorEditor)
    
    
};
