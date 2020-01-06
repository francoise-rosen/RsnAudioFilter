/*
  ==============================================================================

    Tasks:

 PROBLEM: Pointer being deleted has not been allocated (AudioBuffer) - this is important to solve. Appeared after I added OwnedArray<AudioSampleBuffer> waveforms array
    1. Timer CPU + Labels - done
    2. Gain Slider - done
    3. Different Wavetables (tri, rect, saw) + detune (4-12 voices)
    3b. An array or enum with different wavetables;
    4. Multislider (read about audio tread) - for those voices
    5. Spread stereo (voices)
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
    void createSaw9(int);
    void createTri9(int);
    void createRect9(int);
    void normalise(AudioSampleBuffer& table);
    
    

private:
    //==============================================================================
    // wavetable size

    const int tableSize = 1 << 12; // 12 bit
    float level;
    int voices;
    
    AudioSampleBuffer sineTable;
    // try alternatively Array<float> sineTable;
    AudioSampleBuffer saw9Table;
    //AudioSampleBuffer tri9Table;
    AudioSampleBuffer tri9Table;
    AudioSampleBuffer rect9Table;
    
    // an array of oscillators (voices
    OwnedArray<WavetableOscillator_V1> voicesArray;
    
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
    
    // set of filled tables
    OwnedArray<AudioSampleBuffer> waveforms;
    
    void fillTables();
    
    // timer labels
    Label cpuUsageLabel;
    Label cpuUsageText;
    
    // gain slider
    Slider mainGain;
    Label mainGainLabel;
    
    


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
