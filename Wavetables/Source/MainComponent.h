/*
  ==============================================================================

    Tasks:
 SOLVE DROPOUT ON STARTUP WITH BIG BUFFERS WITH SAW AND TRI - solved
 (clear the table before filling it!)
 PROBLEM: Pointer being deleted has not been allocated (AudioBuffer) - this is important to solve. Appeared after I added OwnedArray<AudioSampleBuffer> waveforms array
    1. Timer CPU + Labels - done
    2. Gain Slider - make it smooth!
    3. Different Wavetables (tri, rect, saw) + detune (4-12 voices)
    3b. An array or enum with different wavetables;
    4. Make it work with Phasor!
    4.1 Make another class Wavetable that will hold an array of AudioSampleBuffer waveforms
    4.2 Make an oscillator driven by Phaser and reading buffers from Wavetable
    5. AllPass, additive synth with these waveforms?
    6. AM
    7. FM + index, harmonicity sliders
    8. Waveshaper functions
    9. Simple Waveshaper
    10. Quad waveshaper
    11. LFOs: sine, sah, tri (+angle), rect
 // how about if I want to modulate those harmonics / change ratios etc?
    
  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Wavetable.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/


// ========================================================================
// MainComponent

class MainComponent   : public AudioAppComponent,
                        public Slider::Listener,
                        public Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
        // Timer
    void timerCallback() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;
    
    // Listeners
    
    void sliderValueChanged (Slider* slider) override;
    
    
    // oscillator members
    
    // called in constructor before we start audio processing
    void createSineWavetable();
    void createSaw(int);
    void createTri(int);
    void createRect(int);
    void normalise(AudioSampleBuffer& table);
    
    

private:
    //==============================================================================
    // wavetable size

    const int tableSize = 1 << 12; // 12 bit
    double currentLevel;
    double targetLevel;
    int voices;
    
    // do I need these?
    // or just one AdioSampleBuffer temp ?
    // I send the tables in array anyway!
    AudioSampleBuffer sineTable;
    AudioSampleBuffer sawTable;
    AudioSampleBuffer triTable;
    AudioSampleBuffer rectTable;
    
    // an array of oscillators (voices

    
    //OwnedArray<WavetableOscillator_V1> voicesArray;
    
    //
    enum WaveformType
    {
        sine,
        saw3,
        saw5,
        saw7,
        saw9,
        tri3,
        tri5,
        tri7,
        tri9
    };
    
    WaveformType wave; 

    
    void fillTables();
    
    // or as elem in vector?
    Array<AudioSampleBuffer> waveforms;
    
    OwnedArray<WavetableOscillator_V1> voicesArray;
    
    // timer labels
    Label cpuUsageLabel;
    Label cpuUsageText;
    
    // gain slider
    Slider mainGain;
    Label mainGainLabel;
    
    


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
