/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#define GAIN_NAME "Gain"
#define SATURATION_NAME "Saturate"
#define SYMMETRY_NAME "Symmetry"
#define CROSSFADE_NAME "Crossfade"
#define FUNCTION_A_NAME "Functions_A"
#define FUNCTION_B_NAME "Functions_B"

//==============================================================================
/**
*/
class RaveshaperAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    RaveshaperAudioProcessor();
    ~RaveshaperAudioProcessor();

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
    AudioProcessorValueTreeState& getProcessorValueTreeState();
    
    static String paramGain;
    static String paramSaturate;
    static String paramSymmetry;
    static String paramCrossfade;
    static String paramFunctionsA;
    static String paramFunctionsB;

private:
    //==============================================================================
    AudioProcessorValueTreeState parameters;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RaveshaperAudioProcessor)
};
