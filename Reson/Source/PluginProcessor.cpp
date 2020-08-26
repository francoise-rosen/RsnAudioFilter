/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
// initialise static members - parameter IDs
juce::String ResonAudioProcessor::paramFreq = "cutOffFrequency";
juce::String ResonAudioProcessor::paramQ = "qualityFactor";
juce::String ResonAudioProcessor::paramGain = "gain";
juce::String ResonAudioProcessor::paramBypass = "bypass";
juce::String ResonAudioProcessor::paramAlgorithm = "algorithm";
juce::StringArray ResonAudioProcessor::filterAlgorithms {"BZT", "Analogue", "Simple Resonator", "Symmetrical Resonator"};

//==============================================================================
ResonAudioProcessor::ResonAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
freqAtom{1000.0f}, qAtom{1.0f}, gainAtom{-12.0f},
bypassAtom{true}, // resonant filter is on by default
algorithmAtom{1}, // default - symmetrical resonant filter

//==============================================================================
// initialise the AudioProcessorValueTreeState object
parameters{*this,
    nullptr,
    juce::Identifier("RESON_PARAMETERS"),
    
    // ParameterLayout
    {
    std::make_unique<juce::AudioParameterFloat>(paramFreq,
                                                "FREQUENCY",
                                                juce::NormalisableRange<float>(20.0f, 19000.0f,
                                                    0.01f,
                                                std::log(0.5f) / std::log(1000.0f / 18980.0f)),
                                                freqAtom.get(), "Hz",
                                                juce::AudioProcessorParameter::genericParameter,
                                                [](float val, int) {return juce::String(val,2) + "Hz";},
                                                [](const juce::String& str_value) {return str_value.dropLastCharacters(3).getFloatValue(); }
                                                
                                                                               
        ),
        std::make_unique<juce::AudioParameterFloat>(paramQ,
                                                    "QUALITY_FACTOR",
                                                    0.01f, 200.0f, qAtom.get()),
        std::make_unique<juce::AudioParameterFloat>(paramGain,
                                                    "GAIN",
                                                    juce::NormalisableRange<float>(-100.0f, 12.0f, 0.01f,
                                                                                   std::log(0.5f)/std::log(88.0f / 112.0f)),
                                                    gainAtom.get(), "dB",
                                                    juce::AudioProcessorParameter::genericParameter,
                                                    [](float val, int) {return juce::String(val, 3) + "dB";},
                                                    [](const juce::String& str_value) {return str_value.dropLastCharacters(3).getFloatValue();}
                                                    ),
        std::make_unique<juce::AudioParameterInt>(paramAlgorithm,
                                                  "ALGORITHM",
                                                  0, filterAlgorithms.size(), algorithmAtom.get()),
        std::make_unique<juce::AudioParameterBool>(paramBypass,
                                                   "BYPASS",
                                                   bypassAtom.get())
        
    }
}

//==============================================================================
{
    // add parameter listeners
    parameters.addParameterListener(paramFreq, this);
    parameters.addParameterListener(paramQ, this);
    parameters.addParameterListener(paramGain, this);
    parameters.addParameterListener(paramBypass, this);
    parameters.addParameterListener(paramAlgorithm, this);
    
    lastGain = gainAtom.get();
    
    resonParameters.frequency = freqAtom.get();
    resonParameters.Q = qAtom.get();
    resonParameters.algorithm = static_cast<sfd::FilterAlgorithm>(algorithmAtom.get());
}

ResonAudioProcessor::~ResonAudioProcessor()
{
}

//==============================================================================
void ResonAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    for (int i = 0; i < getTotalNumOutputChannels(); ++i)
    {
        stereoResonator.add(new sfd::Resonator<double>(resonParameters, sampleRate));
    }
    
    
}

void ResonAudioProcessor::releaseResources()
{
  
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ResonAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void ResonAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == paramFreq)
    {
        freqAtom = newValue;
    }
    
    else if (parameterID == paramQ)
    {
        qAtom = newValue;
    }
    
    else if (parameterID == paramGain)
    {
        gainAtom = newValue;
    }
    
    else if (parameterID == paramBypass)
    {
        bypassAtom = (bool)newValue;
    }
    
    else if (parameterID == paramAlgorithm)
    {
        algorithmAtom = (int)newValue;
    }
}

void ResonAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    const float gain = juce::Decibels::decibelsToGain(gainAtom.get());
    const bool bypass = bypassAtom.get();
    
    resonParameters.frequency = freqAtom.get();
    resonParameters.Q = qAtom.get();
    resonParameters.algorithm = static_cast<sfd::FilterAlgorithm>(algorithmAtom.get());

    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        stereoResonator[channel]->setParameters(resonParameters);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            auto inputSample = channelData[sample];
            channelData[sample] = stereoResonator[channel]->process(inputSample) * gain;
        }
    }
}

//==============================================================================
bool ResonAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ResonAudioProcessor::createEditor()
{
    return new ResonAudioProcessorEditor (*this);
}

//==============================================================================
void ResonAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ResonAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
const juce::String ResonAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ResonAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool ResonAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool ResonAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double ResonAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ResonAudioProcessor::getNumPrograms()
{
    return 1;
}

int ResonAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ResonAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ResonAudioProcessor::getProgramName (int index)
{
    return {};
}

void ResonAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================

juce::AudioProcessorValueTreeState& ResonAudioProcessor::getValueTree()
{
    return parameters;
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ResonAudioProcessor();
}


