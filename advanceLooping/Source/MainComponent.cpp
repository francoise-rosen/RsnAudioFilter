/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : Thread ("Background Thread")
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    setAudioChannels (0, 2);
    startThread();
   
}

MainComponent::~MainComponent()
{
    stopThread(4000);
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

// Background Thread

void MainComponent::run()
{
    while(!threadShouldExit())
    {
        checkForBuffersToFree();
        wait(500);
    }
}

void MainComponent::checkForBuffersToFree()
{
    // iterate over buffers
    for(auto i = buffers.size(); --i >= 0;)
    {
        // check how many references each object has
        // saving it in Ptr buffer
        ReferenceCountedBuffer::Ptr buffer(buffers.getUnchecked(i));
        
        // if getReferenceCount is 2 then we know, no other process is using it
        // 1 reference is in buffers array
        // 2nd is saved in this local buffer
        if(buffer->getReferenceCount() == 2)
            buffers.remove(i);
    }
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{

}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{

    bufferToFill.clearActiveBufferRegion();
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

