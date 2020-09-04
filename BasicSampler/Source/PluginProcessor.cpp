/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BasicSamplerAudioProcessor::BasicSamplerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    for (int i = 0; i < numOfVoices; ++i)
    {
        sampler.addVoice(new juce::SamplerVoice());
    }
    
    
    formatManager.registerBasicFormats();
    midiNoteRange.setRange(0, 128, true);
    filePathChanged = false;
}

BasicSamplerAudioProcessor::~BasicSamplerAudioProcessor()
{
}

//==============================================================================

void BasicSamplerAudioProcessor::setFilePath(const juce::File f)
{
    juce::String s = f.getFullPathName();
    filePath.swapWith(s);
}

const juce::String& BasicSamplerAudioProcessor::getFileText()
{
    return fileText;
}

void BasicSamplerAudioProcessor::openButtonClicked()
{
    juce::FileChooser chooser {"Select a file (wav) for playback", {}, "*.wav"};
    
    if (chooser.browseForFileToOpen())
    {
        juce::File file = chooser.getResult();
        auto path = file.getFullPathName();
        filePath.swapWith(path);
        
        // notify()
    }
    
    loadFile();
}

void BasicSamplerAudioProcessor::loadFile()
{
    sampler.clearSounds(); // how about the tread approach?
    
    juce::String pathToOpen;
    pathToOpen.swapWith(filePath);
    
    if (pathToOpen.isNotEmpty())
    {
        juce::File file {pathToOpen};
    
        // create a reader for the file
        std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));
        
        // if format found in formatManager, we can proceed with adding the sound
        // to the sampler
        if (reader.get() != nullptr)
        {
            sampler.addSound(new juce::SamplerSound{"Sample", *reader, midiNoteRange, 60, 0.1, 0.1, 10.0});
            
            // read to waveform
            auto bufferLength = (int)reader->lengthInSamples;
            
            //read max 2 channels in the buffer
            waveform.setSize(1, bufferLength);
            waveform.clear();
            reader->read(&waveform, 0, bufferLength, 0, true, false);
            fileText = file.getFileName();
            filePathChanged = true;
        }
   
    }
    
    else
    {
        fileText = "Please use open button to load an audio file or drag it here.";
    }
}

juce::AudioBuffer<float> BasicSamplerAudioProcessor::getWaveform() const
{
    assert (waveform.getNumSamples() > 0);
    return waveform;
}


const juce::String BasicSamplerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BasicSamplerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BasicSamplerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BasicSamplerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BasicSamplerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BasicSamplerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BasicSamplerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BasicSamplerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BasicSamplerAudioProcessor::getProgramName (int index)
{
    return {};
}

void BasicSamplerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void BasicSamplerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    sampler.setCurrentPlaybackSampleRate(sampleRate);
}

void BasicSamplerAudioProcessor::releaseResources()
{
    
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BasicSamplerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void BasicSamplerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    sampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool BasicSamplerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BasicSamplerAudioProcessor::createEditor()
{
    return new BasicSamplerAudioProcessorEditor (*this);
}

//==============================================================================
void BasicSamplerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
  
}

void BasicSamplerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
 
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BasicSamplerAudioProcessor();
}
