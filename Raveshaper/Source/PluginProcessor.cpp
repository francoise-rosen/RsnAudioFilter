/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


String RaveshaperAudioProcessor::paramGain {"gain"};
String RaveshaperAudioProcessor::paramSaturate {"saturate"};
String RaveshaperAudioProcessor::paramSymmetry {"symmetry"};
String RaveshaperAudioProcessor::paramCrossfade {"crossfade"};
String RaveshaperAudioProcessor::paramFunctionsA {"functions_A"};
String RaveshaperAudioProcessor::paramFunctionsB {"functions_B"};


//==============================================================================
RaveshaperAudioProcessor::RaveshaperAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
: AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                  .withInput  ("Input",  AudioChannelSet::stereo(), true)
#endif
                  .withOutput ("Output", AudioChannelSet::stereo(), true)
#endif
                  ),
parameters(
           *this, nullptr, "PARAMETERS",
           {
               std::make_unique<AudioParameterFloat> (paramGain, GAIN_NAME,
                                                      NormalisableRange<float>(-100.0f, 0.0f), -16.0f),
               std::make_unique<AudioParameterFloat>(paramSaturate, SATURATION_NAME,                         NormalisableRange<float>(0.0f, 1.0f), 0.0f),
               std::make_unique<AudioParameterFloat>(paramSymmetry, SYMMETRY_NAME,
                                                     NormalisableRange<float>(-1.0f, 1.0f), 0.0f),
               std::make_unique<AudioParameterFloat>(paramCrossfade, CROSSFADE_NAME,
                                                     NormalisableRange<float>(-48.0f, 48.0f), 0.0f),
               std::make_unique<AudioParameterChoice>(paramFunctionsA, FUNCTIONS_A_NAME, StringArray({"SFDTanh", "SFDcos", "SFDsine" "SFDclip", "SFDClipCascade"}), 1),
               std::make_unique<AudioParameterChoice>(paramFunctionsA, FUNCTIONS_A_NAME, StringArray({"SFDTanh", "SFDcos", "SFDsine", "SFDclip", "SFDClipCascade"}), 1)
               
           }
           )
#endif
{
}

RaveshaperAudioProcessor::~RaveshaperAudioProcessor()
{
}

//==============================================================================
const String RaveshaperAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool RaveshaperAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool RaveshaperAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool RaveshaperAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double RaveshaperAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int RaveshaperAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int RaveshaperAudioProcessor::getCurrentProgram()
{
    return 0;
}

void RaveshaperAudioProcessor::setCurrentProgram (int index)
{
}

const String RaveshaperAudioProcessor::getProgramName (int index)
{
    return {};
}

void RaveshaperAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void RaveshaperAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    currentBlockSize = samplesPerBlock;
    gain = 0;
}

void RaveshaperAudioProcessor::releaseResources()
{
    
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool RaveshaperAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void RaveshaperAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    gain = *parameters.getRawParameterValue(paramGain);
    

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        
        // sample bookkeeping
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            auto sample = buffer.getSample(channel, i);
            channelData[i] = transferFunction.transform(TransferFunction<float>::Functions::tanh, sample, 3, 1) * Decibels::decibelsToGain(gain);
        }
        
    }
}

//==============================================================================
bool RaveshaperAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* RaveshaperAudioProcessor::createEditor()
{
    return new RaveshaperAudioProcessorEditor (*this);
}

//==============================================================================
void RaveshaperAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void RaveshaperAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

AudioProcessorValueTreeState& RaveshaperAudioProcessor::getProcessorValueTreeState()
{
    return parameters;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RaveshaperAudioProcessor();
}

