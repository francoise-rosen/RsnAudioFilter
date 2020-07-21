/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class BiquadAudioProcessor  : public juce::AudioProcessor,
public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    BiquadAudioProcessor();
    ~BiquadAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

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
    
    //==============================================================================
    juce::AudioProcessorValueTreeState& getValueTree();
    
    //==============================================================================
    // listener's pure virtual
    void parameterChanged(const juce::String& paramID, float newValue) override;
    
    //==============================================================================
    // parameter IDs
    static juce::String freqParam;
    static juce::String qParam;
    static juce::String dryWetParam;
    static juce::String gainParam;
    static juce::String typeParam;
    static juce::String orderParam;
    static juce::String bypassParam;
    static juce::Array<juce::String> filterTypeList;

private:
    
    // wrap the data in Atomic
    juce::Atomic<float> freqAtom{1000.0f};
    juce::Atomic<float> qAtom{1.0f};
    juce::Atomic<float> dryWetAtom{0.0f}; // range 0.0f - 1.0f, show 0 - 100%
    juce::Atomic<float> gainAtom{-12.0f}; // in dB
    juce::Atomic<int> typeAtom{0}; // LPF
    juce::Atomic<int> orderAtom{1};
    juce::Atomic<bool> bypassAtom{false}; // off
    
    juce::AudioProcessorValueTreeState parameters;
    
    // intermediate values for smoothing
    float lastFreq;
    float lastQ;
    float lastDryWet;
    float lastGain; // not in dB!
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BiquadAudioProcessor)
};
