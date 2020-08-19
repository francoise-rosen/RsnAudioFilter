/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
FilterDesignAudioProcessor::FilterDesignAudioProcessor()
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

freqAtom{1000.0}, qAtom{0.7071}, dryWetAtom{0.0},
gainAtom{0.0}, typeAtom{0}, orderAtom{0},
bypassAtom{false},

parameters{*this,
    nullptr,
    juce::Identifier("FILTER_PARAMETERS"),
    //Parameter Layout
    {
        std::make_unique<juce::AudioParameterFloat>(freqParam,
                                                    "FREQUENCY",
                                                    juce::NormalisableRange<float>(20.0, 22000.0, 0.001, std::log(0.5)/std::log(570.0/21980.0)),
                                                    freqAtom.get(), "Hz",
                                                    juce::AudioProcessorParameter::genericParameter,
                                                    [](double value, int) {return juce::String(value, 3) + "Hz";},
                                                    [](const juce::String& s) {return s.dropLastCharacters(3).getFloatValue(); }
                                                    ),
        std::make_unique<juce::AudioParameterFloat>(qParam,
                                                    "QUALITY_FACTOR",
                                                    juce::NormalisableRange<float>(0.02, 40.0, 0.01, std::log(0.5)/std::log(1.0/39.98)),
                                                    qAtom.get()
                                                    ),
        std::make_unique<juce::AudioParameterFloat>(dryWetParam,
                                                    "DRY_WET_GAIN",
                                                    juce::NormalisableRange<float>(0.0, 100.0, 0.1, 1),
                                                    dryWetAtom.get(), "%",
                                                    juce::AudioProcessorParameter::genericParameter,
                                                    [](double value, int) {return juce::String(value, 1) + "%";},
                                                    [](const juce::String s) {return s.dropLastCharacters(1).getFloatValue();}
                                                    ),
        std::make_unique<juce::AudioParameterFloat>(gainParam,
                                                    "OUTPUT_GAIN",
                                                    juce::NormalisableRange<float>(-100.0, 12.0, 0.01, std::log(0.5)/std::log(94.0/112.0)),
                                                    gainAtom.get(), "dB",
                                                    juce::AudioProcessorParameter::genericParameter,
                                                    [](double value, int) {return juce::String(value, 2) + "dB";},
                                                    [](const juce::String s) {return s.dropLastCharacters(3).getFloatValue();}
                                                    ),
        std::make_unique<juce::AudioParameterChoice>(filterTypeParam,
                                                     "FILTER_TYPE",
                                                     algorithmIDs,
                                                     typeAtom.get()
                                                     ),
        std::make_unique<juce::AudioParameterInt>(orderParam,
                                                  "FILTER_ORDER",
                                                  1, 4, orderAtom.get()),
        std::make_unique<juce::AudioParameterBool>(bypassParam,
                                                   "FILTER_BYPASS",
                                                   bypassAtom.get(), "Bypass")
    }
}

{
    parameters.addParameterListener(freqParam, this);
    parameters.addParameterListener(qParam, this);
    parameters.addParameterListener(dryWetParam, this);
    parameters.addParameterListener(gainParam, this);
    parameters.addParameterListener(filterTypeParam, this);
    parameters.addParameterListener(orderParam, this);
    parameters.addParameterListener(bypassParam, this);
    
    lastDryWetGain = dryWetAtom.get();
    lastOutputGain = gainAtom.get();
    
    filterParameters.algorithm = static_cast<rosen::FilterType>(typeAtom.get());
    filterParameters.freq = freqAtom.get();
    filterParameters.Q = qAtom.get();
    //filterParameters.order = orderAtom.get();
    //assert (filterParameters.order > 0);
}

FilterDesignAudioProcessor::~FilterDesignAudioProcessor()
{
}


//==============================================================================
void FilterDesignAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    for (int i = 0; i < getTotalNumInputChannels(); ++i)
    {
        filter.add(new rosen::Filter<double>(filterParameters, sampleRate));
        
    }
}

void FilterDesignAudioProcessor::releaseResources()
{
  
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FilterDesignAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void FilterDesignAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    const double gain = juce::Decibels::decibelsToGain(gainAtom.get());
    const double wet = dryWetAtom.get() * 0.01;
    const double dry = 1.0 - wet;
    filterParameters.algorithm = static_cast<rosen::FilterType>(typeAtom.get());
    filterParameters.freq = freqAtom.get();
    filterParameters.Q = qAtom.get();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        filter[channel]->setParameters(filterParameters);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            double rawSample = channelData[sample];
            double processedSample = filter[channel]->process(static_cast<double>(channelData[sample]));
            channelData[sample] = (rawSample * dry + processedSample * wet) * lastOutputGain;
            
        }
    }
    lastOutputGain = gain;
}

//==============================================================================
bool FilterDesignAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* FilterDesignAudioProcessor::createEditor()
{
    return new FilterDesignAudioProcessorEditor (*this);
}

//==============================================================================
void FilterDesignAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{

}

void FilterDesignAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{

}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FilterDesignAudioProcessor();
}

juce::AudioProcessorValueTreeState& FilterDesignAudioProcessor::getValueTree()
{
    return parameters;
}

void FilterDesignAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue) 
{
    if (parameterID == freqParam)
    {
        freqAtom = static_cast<double>(newValue);
    }
    else if (parameterID == qParam)
    {
        qAtom = static_cast<double>(newValue);
    }
    else if (parameterID == dryWetParam)
    {
        dryWetAtom = static_cast<double>(newValue);
    }
    else if (parameterID == gainParam)
    {
        gainAtom = static_cast<double>(newValue);
    }
    else if (parameterID == filterTypeParam)
    {
        typeAtom = static_cast<int>(newValue);
    }
    else if (parameterID == orderParam)
    {
        orderAtom = static_cast<int>(newValue);
    }
    else if (parameterID == bypassParam)
    {
        bypassAtom = static_cast<bool>(newValue);
    }
}

juce::Array<juce::String> FilterDesignAudioProcessor::algorithmIDs {"LPF6", "HPF6", "LPF12", "HPF12"};
juce::Array<juce::String> FilterDesignAudioProcessor::filterRollOff {"6dB", "12dB"};
juce::String FilterDesignAudioProcessor::freqParam {"frqID"};
juce::String FilterDesignAudioProcessor::qParam {"QID"};
juce::String FilterDesignAudioProcessor::gainParam {"gainID"};
juce::String FilterDesignAudioProcessor::dryWetParam {"dryWetID"};
juce::String FilterDesignAudioProcessor::filterTypeParam {"algoID"};
juce::String FilterDesignAudioProcessor::orderParam {"orderID"};
juce::String FilterDesignAudioProcessor::bypassParam {"bypassID"};

//==============================================================================
const juce::String FilterDesignAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FilterDesignAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool FilterDesignAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool FilterDesignAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double FilterDesignAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FilterDesignAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int FilterDesignAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FilterDesignAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FilterDesignAudioProcessor::getProgramName (int index)
{
    return {};
}

void FilterDesignAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

