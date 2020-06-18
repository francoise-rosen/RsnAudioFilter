/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
// Static parameters
String DubFxAudioProcessor::paramGain = "gain";
String DubFxAudioProcessor::paramDelay = "delay";
String DubFxAudioProcessor::paramFeedback = "feedback";

//==============================================================================
DubFxAudioProcessor::DubFxAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif

gainAtom{-100.0f}, delayAtom{100.0f}, feedbackAtom{-50.0f}, lastGain{0.0f},
lastFeedbackValue{0.0f},

 //initialise parameters
parameters(*this, // processor to connect to
           nullptr, // undo
           Identifier("DELAY_PARAMETERS"),
           {
               std::make_unique<AudioParameterFloat>(paramGain,
                                                     "GAIN",
                                                     NormalisableRange<float>(-100.0f, 6.0f, 0.1f, std::log(0.5f) / std::log(100.0f/106.0f)),
                                                     gainAtom.get(), "dB",
                                                     AudioProcessorParameter::genericParameter,
                                                     [](float val, int) {return String(val, 2) + "dB";},
                                                     [](const String& s) {return s.dropLastCharacters(3).getFloatValue();}
                                                     ),
               std::make_unique<AudioParameterFloat>(paramDelay,
                                                     "DELAY",
                                                     NormalisableRange<float>(0.1f, 2000.0f, 0.1f),
                                                     delayAtom.get(), "ms",
                                                     AudioProcessorParameter::genericParameter,
                                                     [](float val, int) {return String(val) + "ms";},
                                                     [](const String& s) {return s.dropLastCharacters(3).getFloatValue();}
                                                     ),
               std::make_unique<AudioParameterFloat>(paramFeedback,
                                                     "FEEDBACK",
                                                     NormalisableRange<float>(-100.0f, 0.0f, -6.0f, std::log(0.5f) / std::log(100.0f / 106.0f)),
                                                     feedbackAtom.get(), "dB",
                                                     AudioProcessorParameter::genericParameter,
                                                     [](float val, int) {return String(val, 2) + "dB";},
                                                     [](const String& s) {return s.dropLastCharacters(3).getFloatValue();}
                                                     )
           }
           )
{
    // Listeners
    //Attach a callback to one of the parameters, which will be called when the parameter changes.
    parameters.addParameterListener(paramGain, this);
    parameters.addParameterListener(paramDelay, this);
    parameters.addParameterListener(paramFeedback, this);
}

DubFxAudioProcessor::~DubFxAudioProcessor()
{
   
}

//==============================================================================
void DubFxAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    const int bufferLength = 2 * (sampleRate + samplesPerBlock);
    for (int i = 0; i < getTotalNumOutputChannels(); ++i)
    {
        stereoDelay.add(new Delay<float>(bufferLength, sampleRate));
        
    }
    
    for (int i = 0; i < stereoDelay.size(); ++i)
        stereoDelay.getUnchecked(i)->printData();
}

void DubFxAudioProcessor::releaseResources()
{
    
}

// Bus layout. Only Mono -> Stereo or Stereo -> Stereo

bool DubFxAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
    if (layouts.getMainInputChannels() == layouts.getMainInputChannels() > 2)
        return false;
    
    if (layouts.getMainOutputChannels() == layouts.getMainOutputChannels() > 2)
        return false;
    
    if (layouts.getMainInputChannels() > layouts.getMainOutputChannels())
        return false;
    
    return true;
}

void DubFxAudioProcessor::parameterChanged(const String &parameterID, float newValue)
{
    if (parameterID == paramGain)
    {
        gainAtom = newValue; // place newValue in atomic gain
    }
    
    if (parameterID == paramDelay)
    {
        delayAtom = newValue;
    }
    
    if (parameterID == paramFeedback)
    {
        feedbackAtom = newValue;
    }
}

void DubFxAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    // disables denormals on CPU
    ScopedNoDenormals noDenormals;
    
    
    //if (Bus* inputBus = getBus(true, 0))
    
        
    // get values off the atomics
    const float gain = Decibels::decibelsToGain(gainAtom.get());
    const float delayInMs = delayAtom.get(); // distance between write and read pos
    const float feedback = Decibels::decibelsToGain(feedbackAtom.get());
    const float delayAmp = 1.0f;
    
    // at this time no fractional delays
    stereoDelay.getUnchecked(0)->setDelayInMs(delayInMs);
    stereoDelay.getUnchecked(1)->setDelayInMs(delayInMs * 2);
    
    auto totalNumOfInputChannels = getTotalNumInputChannels();
    auto totalNumOfOutputChannels = getTotalNumOutputChannels();

    const auto bufferSize = buffer.getNumSamples();
    
    // for now assume input is stereo
    // WONT READ THE DELAYED SIGNAL
    for (auto channel = 0; channel < stereoDelay.size(); ++channel)
    {
        //int inputChannel = jmin(channel, totalNumOfInputChannels);
        stereoDelay.getUnchecked(channel)->writeToBuffer(buffer, channel, 1.0, 1.0, true);
    }
    
    // read
    
    for (auto channel = 0; channel < totalNumOfOutputChannels; ++channel)
    {
        //int outputChannel = jmin(channel, stereoDelay.size());
        stereoDelay.getUnchecked(channel)->readFromBuffer(buffer, channel, 1.0, 1.0);
    }
    
    
    lastGain = gain;
    
    // add feedback - BLOWS EVEN W/O INPUT
    
//    for (auto outChannel = 0; outChannel < totalNumOfOutputChannels; ++outChannel)
//    {
//        stereoDelay.getUnchecked(outChannel)->writeToBuffer(buffer, outChannel, lastFeedbackValue, feedback, false);
//    }
    
    buffer.applyGainRamp(0, bufferSize, lastGain, gain); // postgain.
    for (auto ddl = 0; ddl < stereoDelay.size(); ++ddl)
        stereoDelay.getUnchecked(ddl)->advanceWritePosition(bufferSize);
    lastFeedbackValue = feedback;
    
    
}


//void SimpleDelayFeedbackAudioProcessor::addFromFeedback(AudioBuffer<float>& buffer)

//==============================================================================
bool DubFxAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DubFxAudioProcessor::createEditor()
{
    return new DubFxAudioProcessorEditor (*this);
}

//==============================================================================
void DubFxAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DubFxAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DubFxAudioProcessor();
}


//==============================================================================
AudioProcessorValueTreeState& DubFxAudioProcessor::accessTreeState()
{
    return parameters;
}

//==============================================================================
const String DubFxAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DubFxAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool DubFxAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool DubFxAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double DubFxAudioProcessor::getTailLengthSeconds() const
{
    return 0.0; // ?
}

int DubFxAudioProcessor::getNumPrograms()
{
    return 1;
}

int DubFxAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DubFxAudioProcessor::setCurrentProgram (int index)
{
}

const String DubFxAudioProcessor::getProgramName (int index)
{
    return {};
}

void DubFxAudioProcessor::changeProgramName (int index, const String& newName)
{
}
