/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
// global tempo?


//Phase Wrap

// like max's phasor, non-bandlimited sawtooth waveform
// this where phase and frequency modulation will be performed
class Phasor{
public:
    Phasor();
    void setFrequency(float frequency, float sampleRate);
    forcedinline float getNextPhase() noexcept; // goes from 0.0 to 1.0
    forcedinline void updateCount() noexcept;
private:
    float currentPhase = 0.0;
    float deltaPhase = 0.0;
    //float currentSampleRate = 0.0;
    
};

class Oscillator{
    // type of oscillator
    // phasor input
public:
    Oscillator();
    Oscillator(Phasor* p); //phase input
    forcedinline float getNextSample() noexcept;
    // forcedinline float modulate(Oscillator* modulator, float index, float harmonicity) noexcept;
    
    // getNextPhase() + modulator.getNextSample()*index
     // first try it w/o modulation
private:
    Phasor* phase;
    
};


class MainComponent   : public AudioAppComponent,
                        public Slider::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    // AudioAppComponent methods
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;
    
    //============================================================================
    void sliderValueChanged(Slider* slider) override;

private:
    //==============================================================================
    // Your private member variables go here...
    float localSampleRate = 0.0f;
    double level = 0.0f;
    Oscillator carrier;
    Oscillator modulator; 
    OwnedArray<Oscillator> oscillators; // additive synth array
    Slider gainSlider;
    float modulationIndex = 0.0f;
   


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
