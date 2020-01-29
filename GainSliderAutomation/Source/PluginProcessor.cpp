/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GainSliderAudioProcessor::GainSliderAudioProcessor()
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
        mainGain(0.0),
        treeState(*this,  // add to this processor
                  nullptr, // ?
                  "PARAMETER", // identifier
                  {std::make_unique<AudioParameterFloat> ( GAIN_ID,
                                                          GAIN_NAME,
                                                          -100.0f,
                                                          0.0f,
                                                          -100.0f)
                  }
                  )

{
}

GainSliderAudioProcessor::~GainSliderAudioProcessor()
{
}

//==============================================================================
const String GainSliderAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GainSliderAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GainSliderAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GainSliderAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GainSliderAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GainSliderAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GainSliderAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GainSliderAudioProcessor::setCurrentProgram (int index)
{
}

const String GainSliderAudioProcessor::getProgramName (int index)
{
    return {};
}

void GainSliderAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void GainSliderAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mainGain = *treeState.getRawParameterValue(GAIN_ID);
    String msg;
    msg << "mainGain in prepare to play: " << String(mainGain);
    std::cout << msg << '\n';
    String msg2;
    msg2 << "target gain here " << String(targetGain);
    std::cout << msg2 << '\n';
    targetGain = mainGain;
}

void GainSliderAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GainSliderAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void GainSliderAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals; // ?
    
    // consider mono -> stereo, stereo -> stereo
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
        
   // auto localTargetGain = targetGain;
    auto localTargetGain = *treeState.getRawParameterValue(GAIN_ID);
    
    // blows the output!
   // mainGain = *treeState.getRawParameterValue(GAIN_ID);
    
    if ( localTargetGain != mainGain)
    {
        auto gainRateOfChange = (localTargetGain - mainGain) / buffer.getNumSamples();
        
        for ( int channel = 0; channel < totalNumOutputChannels; ++ channel)
        {
            auto* channelData = buffer.getWritePointer(channel);
            auto localMainGain = mainGain;
            
            for( int sample = 0;  sample < buffer.getNumSamples(); ++sample)
            {
                localMainGain += gainRateOfChange;
                channelData[sample] = buffer.getSample(channel, sample) * localMainGain;
            }
        }
        mainGain = localTargetGain;
    }
    else
    {
        for ( int channel = 0; channel < totalNumOutputChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer(channel);
            for ( int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                channelData[sample] = buffer.getSample(channel, sample) * mainGain;
            }
            
        }
    }
}

//==============================================================================
bool GainSliderAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* GainSliderAudioProcessor::createEditor()
{
    return new GainSliderAudioProcessorEditor (*this);
}

//==============================================================================
void GainSliderAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void GainSliderAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GainSliderAudioProcessor();
}

//===========================================================

void GainSliderAudioProcessor::setGain(double gain)
{
    targetGain = gain;
}

double GainSliderAudioProcessor::getGain() const
{
    return mainGain;
}

AudioProcessorValueTreeState& GainSliderAudioProcessor::accessTreeState()
{
    return treeState;
}
