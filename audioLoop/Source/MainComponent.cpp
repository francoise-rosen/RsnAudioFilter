/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
  
    formatManager.registerBasicFormats();
    setSize (300, 200);
    
    addAndMakeVisible(&openButton);
    openButton.setButtonText("Open file...");
    openButton.setColour(TextButton::buttonColourId, Colours::green);
    openButton.onClick = [this] {openButtonClicked();};
    
    addAndMakeVisible(&clearButton);
    clearButton.setButtonText("Clear");
    clearButton.setColour(TextButton::buttonColourId, Colours::red);
    clearButton.onClick = [this] {clearButtonClicked();};

    // setting I/O is done during openButtonClicked(). Why?
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // I/O
    
    auto numInputChannels = playbackBuffer.getNumChannels();
    auto numOutputChannels = bufferToFill.buffer->getNumChannels();
    
    auto outputSampleRemaining = bufferToFill.numSamples;
    auto outputSamplesOffset = bufferToFill.startSample;
    
    while(outputSampleRemaining > 0) // we are within a block
    {
        auto bufferSamplesRemaining = playbackBuffer.getNumSamples() - position;
        // in case num of remaining samples in buffer is smaller than the block size
        auto samplesThisTime = jmin(outputSampleRemaining, bufferSamplesRemaining);
        
        for(auto channel = 0; channel < numOutputChannels; ++ channel)
        {
            // fill ouput buffer with data from AudioSampleBuffer
            bufferToFill.buffer->copyFrom(
                                          channel, // destination channel
                                          outputSamplesOffset,
                                          playbackBuffer,
                                          channel % numInputChannels,
                                          position,
                                          samplesThisTime
                                          );
        }
        
        outputSampleRemaining -= samplesThisTime;
        outputSamplesOffset += samplesThisTime;
        position += samplesThisTime;
        
        // looping
        if(position == playbackBuffer.getNumSamples())
            position = 0;
    }
}

void MainComponent::releaseResources()
{

    shutdownAudio();
}

// ==============================

void MainComponent::openButtonClicked()
{
    shutdownAudio();
    
    // open a file using FileChooser
   FileChooser choose ("Open File for playback... ",
                       {},
                       "*.wav");
    
    // this displays a browser
    // this will return true if user selected a file
    if(choose.browseForFileToOpen())
    {
        // if true, use the getResult() to find out what it was
        File file = choose.getResult();
        
        // now we need to be able to read it somehow
        // for that we need to create a AudioFormatReader object (smart pointer) using AudioFormatManager
        
        // AudioFormatReader* AudioFormatManager::createReaderFor(const File &fileName) - searches
        // through the known formats
        std::unique_ptr<AudioFormatReader> reader (formatManager.createReaderFor(file));
        
        auto bps = reader->bitsPerSample;
       // reader->usesFloatingPointData = true;
        std::cout << "This file is " << bps << " beats per sample\n";
        
        // we may want to use reader.usesFloatingPointData = true
        
        // we allow only files shorter than 2 sec
        
        if(reader.get() != nullptr)
        {
            auto duration = reader->lengthInSamples / reader->sampleRate;
            
            if(duration < 2.0)
            {
                // we can read this in AudioSampleBuffer
                playbackBuffer.clear();
                playbackBuffer.setSize(reader->numChannels, (int)reader->lengthInSamples);
                
                std::cout << "readers's num of channels " << reader->numChannels << '\n';
                std::cout << "reader's length in samples " << reader->lengthInSamples << '\n';
                std::cout << "buffer's size " << playbackBuffer.getNumSamples() << '\n';
                
                reader->read(&playbackBuffer, // destination
                             0,
                             (int)reader->lengthInSamples,
                             0,
                             true,
                             true
                             );
                position = 0;
                setAudioChannels(0, reader->numChannels);
                
            }
            else
            {
                std::cout << "The file is longer than 2 seconds!\n";
            }
        }
    }
}

void MainComponent::clearButtonClicked()
{
    shutdownAudio();
}


//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

}

void MainComponent::resized()
{
    auto area = getLocalBounds();
    auto buttonHeight = 30;
    auto buttonWidth = 100;
    auto spacer = 5;
    
    // open button
    openButton.setBounds(area.removeFromTop(buttonHeight).removeFromLeft(buttonWidth).reduced(spacer));
    
    // clear button
    clearButton.setBounds(area.removeFromTop(buttonHeight).removeFromLeft(buttonWidth).reduced(spacer));
}
