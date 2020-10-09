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
class BasicSamplerAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    BasicSamplerAudioProcessor();
    ~BasicSamplerAudioProcessor() override;

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

    
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    void openButtonClicked();
    void loadFile();
    void setFilePath(const juce::File f);
    const juce::String& getFileText();
    juce::AudioBuffer<float> getWaveform() const;
    bool filePathChanged;

private:
    juce::Synthesiser sampler;
    const int numOfVoices = 3;
    
    // File reading - register basic formats
    juce::AudioFormatManager formatManager;
    juce::BigInteger midiNoteRange; // ?
    juce::AudioBuffer<float> waveform;
    
    juce::String filePath;
    juce::String fileText{"Load an audio file..."}; // delegate to Wavethumbnail child
    
    /** In this example, no need to create an ADSR member,
        since it is created and SamplerSound internally (private)
     */
    juce::ADSR::Parameters adsrParams;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicSamplerAudioProcessor)
};
