/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "Resonator.h"


//==============================================================================
/**
*/


class ResonAudioProcessor  : public juce::AudioProcessor,
public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    ResonAudioProcessor();
    ~ResonAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif
    
    void parameterChanged(const juce::String& parameterID, float newValue) override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    juce::AudioProcessorValueTreeState& getValueTree();
    
    // parameter IDs
    static juce::String paramFreq;
    static juce::String paramQ;
    static juce::String paramGain;
    static juce::String paramBypass;
    static juce::String paramAlgorithm;
    static juce::StringArray filterAlgorithms;

private:
    //==============================================================================
    juce::Atomic<float> freqAtom;
    juce::Atomic<float> qAtom;
    juce::Atomic<float> gainAtom;
    juce::Atomic<bool> bypassAtom;
    juce::Atomic<int> algorithmAtom;
    
    float lastGain;
    float lastQValue;
    float lastFreqValue;
    
    juce::OwnedArray<rosen::Resonator<float>> stereoResonator;

    
    juce::AudioProcessorValueTreeState parameters;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResonAudioProcessor)
};
