/*
  ==============================================================================

    This version works
 
 TASK 1
 1. Implement a rotary slider that changes frequency of an oscillator (later text box) - done
 2. Implement a slider that changes the magnitude off the oscillator (vectical bar) - done
 3. Frequency: Octave Switch (Rotary slider with 12 steps range -6 6), Fine Tune (Rotary slider with double values -12.0 0 +12.0) - low priority(knob is done)
 
 How do I smooth several parameters at once? Check my new book for that?
 
 
 TASK 2 Oscillator bank
 
 0. First sort the layout for 8 magnitude sliders, 8 fine tune, 8 octave dials, a label per slider, and every dial
 1. Implement a bank directly in MainComponent? Oscillator must be improved
 2. Expand SineOsc oscillator, Freq sliders, and a magnitude slider?
 3. Smooth all these
 4. GUI layout for 8 oscillators - with Fine Tune, Octave, Detune knob, Magnitude slider
 for each oscillator. Build Rate class
 5. CPU Usage tracker
 
 TASK 3 Harmonic
 
 1. All 8 harmonically related. Introduce Fundamental frequency
 1.1 Implement this in Synthesiser
 2. Envelope per oscillator (when implemented in Synthesiser
 3. Add amplitude and FM modulator
 4. Add waveshaper/saturator
 
 TASK 4 Additive with only fundamental frequency control
 
 1. variable amount of oscillators, multislider
 2. detune (lfo modulated, spread, delay, allpass)
 
 TASK 5. Wavetables for the Oscillator bank from Task 2s

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Oscillator.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class AdditiveSynthComponent   : public AudioAppComponent,
                        public Slider::Listener
{
public:
    //==============================================================================
    AdditiveSynthComponent();
    ~AdditiveSynthComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
    //==============================================================================
    
    void sliderValueChanged (Slider* slider) override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
    std::unique_ptr<Phasor> phasor;
    std::unique_ptr<SineOSC> sineOscillator;
    std::vector<SineOSC*> oscillatorBank;
    
    double frequency;
    double magnitude;
    double gain;
    Slider frequencySlider; // make it rotary
    Slider magnitudeSlider; // make it bar looking
    Range<double> octaveRange{-6.0, 6.0};
    Slider octaveSelector;
    Label frequencyLabel;
    Label magnitudeLabel;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AdditiveSynthComponent)
};
