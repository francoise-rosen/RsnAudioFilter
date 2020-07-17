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
String StereoDelayProcessor::paramGain = "gain";
String StereoDelayProcessor::paramDelayLeft = "delay_left";
String StereoDelayProcessor::paramDelayRight = "delay_right";
String StereoDelayProcessor::paramFeedback = "feedback";
String StereoDelayProcessor::paramType = "delay_type";
String StereoDelayProcessor::paramDryWet = "dryWetGain";
StringArray StereoDelayProcessor::delayTypes {"Stereo", "Ping_Pong"};

//==============================================================================
StereoDelayProcessor::StereoDelayProcessor()
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

gainAtom{-100.0f}, delayLeftAtom{100.0f}, delayRightAtom{100.0f},
feedbackAtom{-50.0f}, typeAtom{0}, lastGain{0.0f}, lastFeedbackValue{0.0f},

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
               std::make_unique<AudioParameterFloat>(paramDryWet,
                                                     "DRY_WET_GAIN",
                                                     NormalisableRange<float>(0.0f, 100.0f, 0.1f, 1),
                                                     dryWetAtom.get(), "%",
                                                     AudioProcessorParameter::genericParameter,
                                                     [](float val, int){return String(val, 2) + "%";},
                                                     [](const String& s) {return s.dropLastCharacters(3).getFloatValue();}
                                                     ),
               std::make_unique<AudioParameterFloat>(paramDelayLeft,
                                                     "DELAYLEFT",
                                                     NormalisableRange<float>(0.1f, 2000.0f, 0.1f),
                                                     delayLeftAtom.get(), "ms",
                                                     AudioProcessorParameter::genericParameter,
                                                     [](float val, int) {return String(val) + "ms";},
                                                     [](const String& s) {return s.dropLastCharacters(3).getFloatValue();}
                                                     ),
               std::make_unique<AudioParameterFloat>(paramDelayRight,
                                                     "DELAYRIGHT",
                                                     NormalisableRange<float>(0.1f, 2000.0f, 0.1f),
                                                     delayRightAtom.get(), "ms",
                                                     AudioProcessorParameter::genericParameter,
                                                     [](float val, int) {return String(val) + "ms";},
                                                     [](const String& s) {return s.dropLastCharacters(3).getFloatValue();}
                                                     ),
               std::make_unique<AudioParameterFloat>(paramFeedback,
                                                     "FEEDBACK",
                                                     NormalisableRange<float>(-100.0f, 0.0f, 0.1f, std::log(0.5f) / std::log(100.0f / 106.0f)),
                                                     feedbackAtom.get(), "dB",
                                                     AudioProcessorParameter::genericParameter,
                                                     [](float val, int) {return String(val, 2) + "dB";},
                                                     [](const String& s) {return s.dropLastCharacters(3).getFloatValue();}
                                                     ),
               std::make_unique<AudioParameterChoice>(paramType,
                                                      "DELAYTYPE",
                                                      delayTypes,
                                                      typeAtom.get() // default index
                                                      )
           }
           )
{
    // Listeners
    //Attach a callback to one of the parameters, which will be called when the parameter changes.
    parameters.addParameterListener(paramGain, this);
    parameters.addParameterListener(paramDelayLeft, this);
    parameters.addParameterListener(paramDelayRight, this);
    parameters.addParameterListener(paramFeedback, this);
    parameters.addParameterListener(paramDryWet, this);
    parameters.addParameterListener(paramType, this);
}

StereoDelayProcessor::~StereoDelayProcessor()
{
   
}

//==============================================================================
void StereoDelayProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    const int bufferLength = 2 * (sampleRate + samplesPerBlock);
    for (int i = 0; i < getTotalNumOutputChannels(); ++i)
    {
        stereoDelay.add(new CircularBuffer<float>(bufferLength, sampleRate));
    }


}

void StereoDelayProcessor::releaseResources()
{
    
}

// Bus layout. Only Mono -> Stereo or Stereo -> Stereo

bool StereoDelayProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
    if (layouts.getMainInputChannels() == layouts.getMainInputChannels() > 2)
        return false;
    
    if (layouts.getMainOutputChannels() == layouts.getMainOutputChannels() > 2)
        return false;
    
    if (layouts.getMainInputChannels() > layouts.getMainOutputChannels())
        return false;
    
    return true;
}

void StereoDelayProcessor::parameterChanged(const String &parameterID, float newValue)
{
    if (parameterID == paramGain)
    {
        gainAtom = newValue; // place newValue in atomic gain
    }
    
    if (parameterID == paramDelayLeft)
    {
        delayLeftAtom = newValue;
    }
    
    if (parameterID == paramDelayRight)
    {
        delayRightAtom = newValue;
    }
    
    if (parameterID == paramFeedback)
    {
        feedbackAtom = newValue;
    }
    
    if (parameterID == paramDryWet)
    {
        dryWetAtom = newValue;
    }
    
    if (parameterID == paramType)
    {
        // index for delay type (0 - stereo, 1 - ping pong ...)
        typeAtom = roundToInt(newValue);
    }
}

void StereoDelayProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    // disables denormals on CPU
    ScopedNoDenormals noDenormals;
    
    //if (Bus* inputBus = getBus(true, 0))
        
    // get values off the atomics
    const float gain = Decibels::decibelsToGain(gainAtom.get());
    const float delayInMsLeft = delayLeftAtom.get(); // distance between write and read pos
    const float delayInMsRight = delayRightAtom.get();
    const float feedback = Decibels::decibelsToGain(feedbackAtom.get());
    const float wetGain = dryWetAtom.get() * 0.01;
    const int delayType = typeAtom.get();
    const float delayAmp = 1.00f; // gain for delayed signals
    
    auto totalNumOfInputChannels = getTotalNumInputChannels();
    auto totalNumOfOutputChannels = getTotalNumOutputChannels();
    const auto bufferSize = buffer.getNumSamples();
    
    // what if I need mono -> stereo? Test it
    for (auto i = totalNumOfInputChannels; i < totalNumOfOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
    //==============================================================================
    // BLOCK
    //==============================================================================
    
    // WRITE TO DELAY
//    for (auto channel = 0; channel < stereoDelay.size(); ++channel)
//    {
//        int inputChannel = jmin(channel, totalNumOfInputChannels);
//        stereoDelay.getUnchecked(channel)->writeBlock(buffer, inputChannel, delayAmp, delayAmp, true);
//    }
//
//    // READ FROM DELAY
//
//    stereoDelay.getUnchecked(0)->readBlock(buffer, 0, delayInMsLeft, lastWetGain, wetGain, Interpolation::linear);
//    stereoDelay.getUnchecked(jmin(1, stereoDelay.size()-1))->readBlock(buffer, 1, delayInMsRight, lastWetGain, wetGain, Interpolation::linear);
//
//    // WRITE FEEDBACK TO DELAY
//
//     //This works only if there're 2 delay buffers and 2 output channels
//    for (auto outChannel = 0; outChannel < totalNumOfOutputChannels; ++outChannel)
//    {
//        // Stereo is a default setting, delayType == 0
//        int delayChannel = outChannel;
//        //PingPong
//        if (delayType == 1)
//        {
//            // flip the channels
//            delayChannel = (outChannel == 0) ? 1 : 0;
//        }
//        // write back to the circular buffer
//        stereoDelay.getUnchecked(delayChannel)->writeBlock(buffer, outChannel, lastFeedbackValue, feedback, false);
//    }
//
//    // advance the writing positon for all the circular buffers
//    for (auto ddl = 0; ddl < stereoDelay.size(); ++ddl)
//        stereoDelay.getUnchecked(ddl)->advanceWrite(bufferSize);
//
//    buffer.applyGainRamp(0, bufferSize, lastGain, gain); // postgain.
//    lastGain = gain;
//    lastWetGain = wetGain;
//    lastFeedbackValue = feedback;
    
    //==============================================================================
    // SAMPLE
    //==============================================================================
    // dry/wet, filter on delay, modulated delay time
    for (auto index = 0; index < bufferSize; ++index)
    {
        for (auto channel = 0; channel < totalNumOfOutputChannels; ++channel)
        {
            auto inputChannel = jmin(channel, totalNumOfInputChannels); /* mono / stereo */
            auto* inputData = buffer.getReadPointer(inputChannel);
            auto* outputData = buffer.getWritePointer(channel);
            
            // read direct and delayed signals
            float inputSample = inputData[index];
            float delayInMs = (channel == 0) ? delayInMsLeft : delayInMsRight;
            float delayedSample = stereoDelay.getUnchecked(channel)->read(delayInMs, Interpolation::linear);
            
            // write to circularBuffer
            float writeToDelayBuffer = inputSample + delayedSample * feedback;
            
  // this does not work for pingpong
//            int delayChannel = channel;
//            if (delayType == 1)
//            {
//                delayChannel = (channel == 0) ? 1 : 0;
//            }
//            stereoDelay.getUnchecked(delayChannel)->write(writeToDelayBuffer);
      
            
// this works for ping pong
            if (delayType == 0) /* stereo */
            {
                stereoDelay.getUnchecked(channel)->write(writeToDelayBuffer);
            }
            else if (delayType == 1) /* ping pong */
            {
                if (channel == 0)
                    stereoDelay.getUnchecked(1)->write(writeToDelayBuffer);
                else if (channel == 1)
                    stereoDelay.getUnchecked(0)->write(writeToDelayBuffer);
            }
            
            // output
            outputData[index] = (inputSample * (1 - wetGain) + delayedSample * wetGain) * gain;
        }
        
    }

}

//==============================================================================
bool StereoDelayProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* StereoDelayProcessor::createEditor()
{
    return new StereoDelayProcessorEditor (*this);
}

//==============================================================================
void StereoDelayProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void StereoDelayProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StereoDelayProcessor();
}


//==============================================================================
AudioProcessorValueTreeState& StereoDelayProcessor::accessTreeState()
{
    return parameters;
}

//==============================================================================
const String StereoDelayProcessor::getName() const
{
    return JucePlugin_Name;
}

bool StereoDelayProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool StereoDelayProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool StereoDelayProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double StereoDelayProcessor::getTailLengthSeconds() const
{
    return 0.0; // ?
}

int StereoDelayProcessor::getNumPrograms()
{
    return 1;
}

int StereoDelayProcessor::getCurrentProgram()
{
    return 0;
}

void StereoDelayProcessor::setCurrentProgram (int index)
{
}

const String StereoDelayProcessor::getProgramName (int index)
{
    return {};
}

void StereoDelayProcessor::changeProgramName (int index, const String& newName)
{
}
