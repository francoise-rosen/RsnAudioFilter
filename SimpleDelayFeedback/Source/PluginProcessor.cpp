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
String SimpleDelayFeedbackAudioProcessor::paramGain = "gain";
String SimpleDelayFeedbackAudioProcessor::paramDelay = "delay";
String SimpleDelayFeedbackAudioProcessor::paramFeedback = "feedback";

//==============================================================================
SimpleDelayFeedbackAudioProcessor::SimpleDelayFeedbackAudioProcessor()
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
writePosition{0}, readPosition{0},

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

SimpleDelayFeedbackAudioProcessor::~SimpleDelayFeedbackAudioProcessor()
{
   
}

//==============================================================================
void SimpleDelayFeedbackAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    const int bufferLength = 2 * (sampleRate + samplesPerBlock);
    delayBuffer.setSize(getTotalNumOutputChannels(), bufferLength);
    delayBuffer.clear();
}

void SimpleDelayFeedbackAudioProcessor::releaseResources()
{
    
}

// Bus layout. Only Mono -> Stereo or Stereo -> Stereo

bool SimpleDelayFeedbackAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
    if (layouts.getMainInputChannels() == layouts.getMainInputChannels() > 2)
        return false;
    
    if (layouts.getMainOutputChannels() == layouts.getMainOutputChannels() > 2)
        return false;
    
    if (layouts.getMainInputChannels() > layouts.getMainOutputChannels())
        return false;
    
    return true;
}

void SimpleDelayFeedbackAudioProcessor::parameterChanged(const String &parameterID, float newValue)
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

void SimpleDelayFeedbackAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    // disables denormals on CPU
    ScopedNoDenormals noDenormals;
    
    
    //if (Bus* inputBus = getBus(true, 0))
    
        
    // get values off the atomics
    const float gain = Decibels::decibelsToGain(gainAtom.get());
    const float delayInMs = delayAtom.get(); // distance between write and read pos
    const float feedback = feedbackAtom.get();
    
    auto totalNumOfInputChannels = getTotalNumInputChannels();
    auto totalNumOfOutputChannels = getTotalNumOutputChannels();
    
    // assume for mono in we'll use mono out
    for (auto i = totalNumOfInputChannels; i < totalNumOfOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
    
    // WHAT IF THE DELAY TIME IS SHORTER THAN THE BLOCK? TEST IT
    // 1. read from delay buffer, write to it then.
    auto bufferSize = buffer.getNumSamples();
    auto delayBufferSize = delayBuffer.getNumSamples();
    
    // Does not work!
    for (auto channel = 0; channel < totalNumOfInputChannels; ++channel)
    {
        auto* bufferData = buffer.getReadPointer(channel);
        auto* delayBufferData = delayBuffer.getReadPointer(channel);
        auto* bufferWrite = buffer.getWritePointer(channel);
        
        // wrap the write position
        if (writePosition > delayBufferSize - 1) writePosition -= delayBufferSize;
        
        readPosition = roundToInt(writePosition - (currentSampleRate * delayInMs / 1000));
        
        if (readPosition < 0) readPosition += delayBufferSize; // wrap the read position
    
        buffer.applyGainRamp(channel, 0, bufferSize, lastGain, gain);
    
        auto readSamplesRemaining = delayBufferSize - readPosition - 1;
        auto writeSamplesRemaining = delayBufferSize - writePosition - 1;
        
        // first write (we have max time + samplesPerBlock for that)
        if (bufferSize < writeSamplesRemaining)
        {
            delayBuffer.copyFromWithRamp(channel, writePosition, bufferData, bufferSize, lastGain, gain);
        }
        
        else
        {
            delayBuffer.copyFromWithRamp(channel, writePosition, bufferData, writeSamplesRemaining, lastGain, gain);
            delayBuffer.copyFromWithRamp(channel, 0, bufferData, bufferSize - writeSamplesRemaining, gain, gain);
        }
        
        // read
        
        if (bufferSize < readSamplesRemaining)
        {
            buffer.addFrom(channel, 0, delayBufferData, bufferSize);
        }
        
        else
        {
            buffer.addFrom(channel, 0, delayBufferData, readSamplesRemaining);
            buffer.addFrom(channel, readSamplesRemaining, delayBufferData, bufferSize - readSamplesRemaining);
        }

        
    }
        lastGain = gain;
        writePosition += bufferSize;
        //readPosition += bufferSize;
    // 2. when the delay buffer is full wrap it and start from 0 sample
    // 3. read from delay buffer
    
    
    

}

//==============================================================================
bool SimpleDelayFeedbackAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SimpleDelayFeedbackAudioProcessor::createEditor()
{
    return new SimpleDelayFeedbackAudioProcessorEditor (*this);
}

//==============================================================================
void SimpleDelayFeedbackAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SimpleDelayFeedbackAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleDelayFeedbackAudioProcessor();
}


//==============================================================================
AudioProcessorValueTreeState& SimpleDelayFeedbackAudioProcessor::accessTreeState()
{
    return parameters;
}

//==============================================================================
const String SimpleDelayFeedbackAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimpleDelayFeedbackAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool SimpleDelayFeedbackAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool SimpleDelayFeedbackAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double SimpleDelayFeedbackAudioProcessor::getTailLengthSeconds() const
{
    return 0.0; // ?
}

int SimpleDelayFeedbackAudioProcessor::getNumPrograms()
{
    return 1;
}

int SimpleDelayFeedbackAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimpleDelayFeedbackAudioProcessor::setCurrentProgram (int index)
{
}

const String SimpleDelayFeedbackAudioProcessor::getProgramName (int index)
{
    return {};
}

void SimpleDelayFeedbackAudioProcessor::changeProgramName (int index, const String& newName)
{
}
