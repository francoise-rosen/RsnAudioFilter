/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Delay.h"
#include "Filter.h"

//==============================================================================
/**
*/
class DubFxAudioProcessor  : public AudioProcessor,
public AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    DubFxAudioProcessor();
    ~DubFxAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void parameterChanged (const String& parameterID, float newValue) override;

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
    
    //==============================================================================
    AudioProcessorValueTreeState& accessTreeState();
    
    static String paramGain;
    static String paramDelay; // in ms
    static String paramFeedback;

private:
    // std::atomic wrappers
    Atomic<float> gainAtom;
    Atomic<float> delayAtom;
    Atomic<float> feedbackAtom;
    
    float lastGain;
    float lastFeedbackValue;
    double currentSampleRate { 0.0 };
    
    OwnedArray<Delay<float>> stereoDelay; // left and right delays
    AudioProcessorValueTreeState parameters;

    UndoManager undo;
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DubFxAudioProcessor)
};
