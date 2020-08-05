/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================


BiquadAudioProcessor::BiquadAudioProcessor()
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
parameters{*this,
           nullptr,
           juce::Identifier("FILTER_PARAMETERS"),
           // ParameterLayout
           {
               std::make_unique<juce::AudioParameterFloat>(freqParam,
                                                           "FREQUENCY",
                                                           juce::NormalisableRange<float>{20.0f, 22000.0f, 0.001f, std::log(0.5f) / std::log(1000.0f/20980.0f)},
                                                               freqAtom.get(),// default
                                                               "Hz",
                                                           juce::AudioProcessorParameter::genericParameter,
                                                           [](float val, int) {return juce::String(val, 2) + "Hz";},
                                                           [](const juce::String& str_val) {return str_val.dropLastCharacters(3).getFloatValue();}
            ),
               std::make_unique<juce::AudioParameterFloat>(qParam,
                                                           "QUALITY_FACTOR",
                                                           juce::NormalisableRange<float>{
                                                               0.1f, 20.0f, 0.01f, std::log(0.5f) / std::log(4.0f / 24.0f)},
                                                           qAtom.get()
                                                           ),
               std::make_unique<juce::AudioParameterFloat>(dryWetParam,
                                                           "DRYWET_FILTER",
                                                           juce::NormalisableRange<float>{0.0f, 100.0f, 0.1f, 1},
                                                           dryWetAtom.get(), "%",
                                                           juce::AudioProcessorParameter::genericParameter,
                                                           [](float val, int) {return juce::String(val, 2) + "%";},
                                                           [](const juce::String& str_val) {return str_val.dropLastCharacters(3).getFloatValue();}
                                                           ),
               std::make_unique<juce::AudioParameterFloat>(gainParam,
                                                           "GAIN",
                                                           juce::NormalisableRange<float>{
                                                               -100.0f, 12.0f, 0.01f, std::log(0.5f)/std::log(88.0f/112.0f)
                                                           },
                                                           gainAtom.get(), "dB",
                                                           juce::AudioProcessorParameter::genericParameter,
                                                           [](float val, int) {return juce::String(val, 2) + "dB";},
                                                           [](const juce::String& strval) {return strval.dropLastCharacters(3).getFloatValue();}
                                                           ),
               std::make_unique<juce::AudioParameterChoice>(typeParam,
                                                            "FILTER_TYPE",
                                                            filterTypeList,
                                                            typeAtom.get()),
               std::make_unique<juce::AudioParameterInt>(orderParam,
                                                         "FILTER_ORDER",
                                                         0, 1, orderAtom.get()),
               std::make_unique<juce::AudioParameterBool>(bypassParam,
                                                          "FILTER_BYPASS",
                                                          bypassAtom.get(),  "Bypass")
           }
}
{
    // add listeners to all the parameters 
    parameters.addParameterListener(freqParam, this);
    parameters.addParameterListener(qParam, this);
    parameters.addParameterListener(dryWetParam, this);
    parameters.addParameterListener(gainParam, this);
    parameters.addParameterListener(typeParam, this);
    parameters.addParameterListener(orderParam, this);
    parameters.addParameterListener(bypassParam, this);
}

BiquadAudioProcessor::~BiquadAudioProcessor()
{
}


//==============================================================================

void BiquadAudioProcessor::parameterChanged(const juce::String &paramID
                                            , float newValue)
{
    // SLIDERS
    if (paramID == freqParam)
    {
        freqAtom = newValue;
    }
    
    else if (paramID == qParam)
    {
        qAtom = newValue;
    }
    
    else if (paramID == dryWetParam)
    {
        dryWetAtom = newValue;
    }
    
    else if (paramID == gainParam)
    {
        gainAtom = newValue;
    }
    
    // COMBO
    
    else if (paramID == typeParam)
    {
        typeAtom = (int)newValue;
    }
    
    else if (paramID == orderParam)
    {
        orderAtom = (int)newValue;
    }
    
    else if (paramID == bypassParam)
    {
        bypassAtom = (int)newValue;
    }
}
void BiquadAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    lastGain = gainAtom.get();
    lastFreq = freqAtom.get();
    lastQ = qAtom.get();
    lastDryWet = dryWetAtom.get();
    
    for (int i = 0; i < getTotalNumInputChannels(); ++i)
    {
        biquadSet.add(new rosen::Biquad<float>(lastFreq, 1, sampleRate, 1, rosen::biquadAlgorithm::LPF));
    }
}

void BiquadAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

// MONO -> MONO, STEREO -> STEREO
#ifndef JucePlugin_PreferredChannelConfigurations
bool BiquadAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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


void BiquadAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    const float gain = juce::Decibels::decibelsToGain(gainAtom.get());
    const float freq = freqAtom.get();
    const float q = qAtom.get();
    const float dryWet = dryWetAtom.get();
    const int algo = typeAtom.get();
    

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        biquadSet[channel]->setParameters(freq, q, algo);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            auto filteredSample = biquadSet[channel]->process(channelData[sample]);
            channelData[sample] = filteredSample * gain;
        }

    }
}

//==============================================================================
bool BiquadAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BiquadAudioProcessor::createEditor()
{
    return new BiquadAudioProcessorEditor (*this);
}

//==============================================================================
void BiquadAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void BiquadAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
const juce::String BiquadAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BiquadAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool BiquadAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool BiquadAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double BiquadAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BiquadAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int BiquadAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BiquadAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BiquadAudioProcessor::getProgramName (int index)
{
    return {};
}

void BiquadAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BiquadAudioProcessor();
}

juce::AudioProcessorValueTreeState& BiquadAudioProcessor::getValueTree()
{
    return parameters;
}

//==============================================================================
// static members
juce::String BiquadAudioProcessor::freqParam {"frequencyID"};
juce::String BiquadAudioProcessor::qParam {"qualityFactorID"};
juce::String BiquadAudioProcessor::dryWetParam {"dryWetID"};
juce::String BiquadAudioProcessor::gainParam {"gainID"};
juce::String BiquadAudioProcessor::typeParam {"filterTypeID"};
juce::String BiquadAudioProcessor::orderParam {"filterOrderID"};
juce::String BiquadAudioProcessor::bypassParam {"bypassID"};
juce::Array<juce::String> BiquadAudioProcessor::filterTypeList {"LPF", "HPF", "LPF2", "HPF2", "ButterLPF2", "ButterHPF"};
juce::Array<juce::String> BiquadAudioProcessor::orderRollOff {"6dB", "12dB"};
