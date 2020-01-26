/*
  ==============================================================================
PHASE A
       1. Level in dB - done
       1.1 Fix this - won't remember the state when the window is closed
       2. smooth the changes - done
       3. automation. Works only if Window is on. Apparently takes the level from GUI. Must take from treeState tho?
       4. save settings
       5. set up the plugin host
 
 PHASE B
 1. Pan dial
 2. Waveshaper (TANH) and a slider (rotary) for amount

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
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
    AudioProcessorValueTreeState treeState;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainSliderAudioProcessor)
};
