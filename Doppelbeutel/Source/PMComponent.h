/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PhaseModulation.h"

//==============================================================================
/*

    TASK 1
    1. Phasor class (only phasor there)
    2. Sine class (receives a Phasor)
    3. Simple PM with Carrier, Modulator, Index
    
    GUI:
    1. Output gain
    2. Carrier level, frequency (30, 500)
    3. Modulator output range (0, pi/2)
    4. Modulator frequency (30, 500)
    
    
    TASK 2
    1. Signal base class -> Phasor -> Rate
    2. Harmonicity factor (user Phasor -> Rate for modulator)
    3. Replace Sine with Wavetable, a hybrid oscillator with Waveshaper
    
    TASK 3
    1. Oversampling
    2. Feedback Path
    3. Add other FM topologies
 
    TASK 4
    1. Synthesiser Midi
    2. dsp::Synthesiser - make a functional synth pluging
    
    TASK 5
    1. Optimisation
    2. Filter Block, LPG
*/
class PMComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    PMComponent();
    ~PMComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PMComponent)
};
