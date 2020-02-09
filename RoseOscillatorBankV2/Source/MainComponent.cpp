/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    frequency = 440;
    phasor.reset(new Phasor(frequency, sampleRate));
    sineOscillator.reset(new SineOSC(phasor.get()));

}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    auto* leftOutput = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* rightOutput = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
    
    gain = 0.25;

    bufferToFill.clearActiveBufferRegion();
    
    for( int sample = 0; sample < bufferToFill.numSamples; ++sample)
    {
        auto sampleValue = sineOscillator->getNextSample();
        leftOutput[sample] = sampleValue * gain;
        rightOutput[sample] = sampleValue * gain;
    }
}

void MainComponent::releaseResources()
{

}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
 
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    
}

void MainComponent::resized()
{

}
