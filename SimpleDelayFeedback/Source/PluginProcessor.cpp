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
lastFeedbackValue{0.0f}, writePosition{0}, readPosition{0},

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
    const float feedback = Decibels::decibelsToGain(feedbackAtom.get());
    const float delayAmp = 1.0f;
    
    // at this time no fractional delays
    const int delayInSmps = roundToInt(delayInMs * currentSampleRate / 1000.0);
    
    auto totalNumOfInputChannels = getTotalNumInputChannels();
    auto totalNumOfOutputChannels = getTotalNumOutputChannels();
    
    // assume for mono in we'll use mono out
    for (auto i = totalNumOfInputChannels; i < totalNumOfOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
    
    // WHAT IF THE DELAY TIME IS SHORTER THAN THE BLOCK? TEST IT

    const auto bufferSize = buffer.getNumSamples();
    const auto delayBufferSize = delayBuffer.getNumSamples();
    
    if (writePosition > delayBufferSize - 1) writePosition -= delayBufferSize;
    
    readPosition = writePosition - delayInSmps;
    
    if (readPosition < 0) readPosition += delayBufferSize;
    
    for (auto channel = 0; channel < totalNumOfInputChannels; ++channel)
    {
        // read pointers to buffer and delay buffer
        
        // write and read
        fillDelayBuffer(buffer, channel, writePosition, delayBufferSize, delayAmp, delayAmp);
        readFromDelayBuffer(buffer, channel, delayBufferSize, readPosition, delayAmp, delayAmp);
       
        
    }
    lastGain = gain;
    
    // add feedback
    // read from output channels
    
    for (auto outChannel = 0; outChannel < totalNumOfOutputChannels; ++outChannel)
    {
         fillDelayBuffer(buffer, outChannel, writePosition, delayBufferSize, lastFeedbackValue, feedback, false);
    }
    
    buffer.applyGainRamp(0, bufferSize, lastGain, gain); // postgain.
    writePosition += bufferSize;
    lastFeedbackValue = feedback;
    
    

}

// fill the delay buffer

void SimpleDelayFeedbackAudioProcessor::fillDelayBuffer(AudioBuffer<float>& buffer, const int& inputChannel, const int& writePosition, const int& delayBufferSize, float gainStart, float gainEnd, bool replace) noexcept
{
    auto* bufferData = buffer.getReadPointer(inputChannel);
    const int samplesLeft = delayBufferSize - writePosition;
    
    // we have enough samples left to place entire block
    if (buffer.getNumSamples() <= samplesLeft)
    {
        if (replace == true){
        delayBuffer.copyFromWithRamp(inputChannel, writePosition, bufferData, buffer.getNumSamples(), gainStart, gainEnd);
        }
        else{
            delayBuffer.addFromWithRamp(inputChannel, writePosition, bufferData, buffer.getNumSamples(), gainStart, gainEnd);
        }
    }
    
    // we have not enough samples in the delayBuffer, so we fill remaining samples
    // and start over from the beginning
    else
    {
        auto crossPos = samplesLeft / buffer.getNumSamples();
        auto crossGain = jmap(static_cast<float>(crossPos), gainStart, gainEnd);
        
        if (replace == true)
        {
        delayBuffer.copyFromWithRamp(inputChannel, writePosition, bufferData, samplesLeft, gainStart, crossGain);
        delayBuffer.copyFromWithRamp(inputChannel, 0, bufferData + samplesLeft, buffer.getNumSamples() - samplesLeft, crossGain, gainEnd);
        }
        else {
            delayBuffer.addFromWithRamp(inputChannel, writePosition, bufferData, samplesLeft, gainStart, gainEnd);
            delayBuffer.addFromWithRamp(inputChannel, 0, bufferData + samplesLeft, buffer.getNumSamples() - samplesLeft, gainStart, gainEnd);
        }
    }
}


// read from delay buffer

void SimpleDelayFeedbackAudioProcessor::readFromDelayBuffer(AudioBuffer<float>& buffer, const int& outputChannel, const int& delayBufferSize, const int& readPosition, float gainStart, float gainEnd) noexcept
    {
        auto* delayBufferData = delayBuffer.getReadPointer(outputChannel);
        auto samplesLeft = delayBufferSize - readPosition;

        // we have enough samples to read before the end of the delayBuffer
        if (buffer.getNumSamples() <= samplesLeft)
        {
            buffer.addFromWithRamp(outputChannel, 0, delayBufferData + readPosition, buffer.getNumSamples(), gainStart, gainEnd);
        }
        
        else
        {
            auto crossPos = samplesLeft / buffer.getNumSamples();
            auto crossGain = jmap(static_cast<float>(crossPos), gainStart, gainEnd);
            buffer.addFromWithRamp(outputChannel, 0, delayBufferData + readPosition, samplesLeft, gainStart, crossGain);
            buffer.addFromWithRamp(outputChannel, samplesLeft, delayBufferData, buffer.getNumSamples() - samplesLeft, crossGain, gainEnd);
        }
    
    }


// feedback fb * y (n - d)

//void SimpleDelayFeedbackAudioProcessor::addFromFeedback(AudioBuffer<float>& buffer)

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
