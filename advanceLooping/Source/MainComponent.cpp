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
    
    addAndMakeVisible(&openButton);
    openButton.setButtonText("Open...");
    openButton.onClick = [this]{openButtonClicked();};
    
    addAndMakeVisible(&clearButton);
    clearButton.setButtonText("Clear");
    clearButton.onClick = [this]{clearButtonClicked();};
    
    formatManager.registerBasicFormats();
    

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
    
    // I'm here
    // first retain a current buffer
    ReferenceCountedBuffer::Ptr retainedBuffer (currentBuffer);
    if(retainedBuffer == nullptr){
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    
    // set I/O
    AudioSampleBuffer* currentAudioSampleBuffer = retainedBuffer->getAudioSampleBuffer();
    auto numInputChannels = currentAudioSampleBuffer->getNumChannels();
    auto numOutputChannels = bufferToFill.buffer->getNumChannels();
    
    auto position = retainedBuffer->position;
    
    auto outputSamplesRemaining = bufferToFill.numSamples;
    auto outputSamplesOffset = 0;
    
    while(outputSamplesRemaining > 0)
    {
        auto bufferSamplesRemaining = currentAudioSampleBuffer->getNumSamples();
        // iterate over channles
    }
    
}

void MainComponent::releaseResources()
{
    currentBuffer = nullptr;
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

}

void MainComponent::resized()
{
    openButton.setBounds(10, 10, getWidth()-20, 40);
    clearButton.setBounds(10, 55, getWidth()-20, 40);

}

void MainComponent::openButtonClicked()
{
    FileChooser chooser ("Open a file for playing back ",
                         {},
                         "*.wav");
    if (chooser.browseForFileToOpen())
    {
        File file = chooser.getResult();
        std::unique_ptr<AudioFormatReader> reader (formatManager.createReaderFor(file));
        
        if (reader.get() != nullptr)
        {
            auto duration = reader->lengthInSamples / reader->sampleRate;
            if (duration < 2)
            {
                ReferenceCountedBuffer::Ptr newBuffer = new ReferenceCountedBuffer(file.getFileName(),
                    (int)reader->numChannels,
                    (int)reader->lengthInSamples);
                
                reader->read(newBuffer->getAudioSampleBuffer(),
                             0,
                             (int)reader->lengthInSamples,
                             0,
                             true, true);
                currentBuffer = newBuffer;
                buffers.add(newBuffer);
            }
            
            else
            {
                std::cout << "Please choose a shorter file\n";
            }
        }
    }
}

void MainComponent::clearButtonClicked()
{
    currentBuffer = nullptr;
}

