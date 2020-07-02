/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CircularBuffer.h"

//==============================================================================
/**
*/
class StereoDelayProcessor  : public AudioProcessor,
public AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    StereoDelayProcessor();
    ~StereoDelayProcessor();

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
    static String paramDelayLeft; // in ms
    static String paramDelayRight; // in ms
    static String paramFeedback;
    static String paramType;
    static String paramDryWet;
    static StringArray delayTypes;

private:
    // std::atomic wrappers
    Atomic<float> gainAtom;
    Atomic<float> delayLeftAtom;
    Atomic<float> delayRightAtom;
    Atomic<float> feedbackAtom;
    Atomic<float> dryWetAtom;
    Atomic<int> typeAtom;
    
    float lastGain;
    float lastFeedbackValue;
    float lastWetGain;
    double currentSampleRate { 0.0 };
    
    OwnedArray<CircularBuffer<float>> stereoDelay; // left and right delays
    AudioProcessorValueTreeState parameters;

    UndoManager undo;
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StereoDelayProcessor)
};
