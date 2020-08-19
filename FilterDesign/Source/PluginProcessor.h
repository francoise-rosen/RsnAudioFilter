/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Filter.h"

//==============================================================================
/**
*/

class FilterDesignAudioProcessor  : public juce::AudioProcessor,
public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    FilterDesignAudioProcessor();
    ~FilterDesignAudioProcessor() override;

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
    // Parameters
    static juce::Array<juce::String> algorithmIDs;
    static juce::Array<juce::String> filterRollOff;
    static juce::String freqParam;
    static juce::String qParam;
    static juce::String gainParam;
    static juce::String dryWetParam;
    static juce::String filterTypeParam;
    static juce::String orderParam;
    static juce::String bypassParam;
    
    juce::AudioProcessorValueTreeState& getValueTree();
    
    //==============================================================================
    void parameterChanged (const juce::String& parameterID, float newValue) override;

private:
    //==============================================================================
    juce::OwnedArray<rosen::Filter<double>> filter;
    
    // parameter data wrapped in atomic
    juce::Atomic<double> freqAtom, qAtom, dryWetAtom, gainAtom;
    juce::Atomic<int> typeAtom, orderAtom;
    juce::Atomic<bool> bypassAtom;
    
    double lastDryWetGain, lastOutputGain;
    rosen::FilterParameters<double> filterParameters;
    
    // processor's value tree state
    juce::AudioProcessorValueTreeState parameters;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterDesignAudioProcessor)
};
