/*
  ==============================================================================
PHASE A
       1. Level in dB - done
       1.1 Fix this - won't remember the state when the window is closed - fixed (removed listener)
       2. smooth the changes - done
       3. automation. Works only if Window is on. Apparently takes the level from GUI. Must take from treeState tho? - fixed, see 1.1
       4. save settings -
       5. set up the plugin host
       6. skew problem (low priority)
 
 PHASE B - make a waveshaper out of it
    1. Pan dial
    2. Waveshaper (TANH) and a slider (rotary) for amount - high priority
    3. Math functions (cos, soft clip, different sigmoid and piecewise functions)
    4. Custom wavetables -> transfer functions
    5. LPG to smooth the saturation
    6. LPF / HPF before the waveshaping stage
    7. DCblock
    8. Oversampling
    9. EPTR for transitions
 
 
 PHASE C:
    1 LFO Bank (-> symmetry, crossfade, index of modulation)
    2 Assymetrical modulation (dial when in center - symetric, either side will modulate negative or positive values)
    this can be modulated by LFO for better result!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "TransferFunction.h"
#define GAIN_ID "gain"
#define GAIN_NAME "Gain"

//==============================================================================
/**
*/
class GainSliderAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    GainSliderAudioProcessor();
    ~GainSliderAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //===============================================================================
    void setGain(double);
    double getGain() const;
    AudioProcessorValueTreeState& accessTreeState();

private:
    double mainGain;
    double targetGain;
    AudioProcessorValueTreeState parameters;
    TransferFunction transferFunction; // so far just one
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainSliderAudioProcessor)
};
