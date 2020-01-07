/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
:state(Stopped)
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);
    
    // we must register basic formats for AudioFormatManager
    // it'll add the list of formats that come with JUCE
    formatManager.registerBasicFormats();
    
    //GUI
    transportSource.addChangeListener(this);
    
    addAndMakeVisible(&openButton);
    openButton.setButtonText("Open...");
    openButton.onClick = [this] {openButtonClicked();};
    
    addAndMakeVisible(&playButton);
    playButton.setButtonText("Play");
    playButton.onClick = [this] {playButtonClicked();};
    playButton.setColour(TextButton::buttonColourId, Colours::green);
    playButton.setEnabled(false);
    
    addAndMakeVisible(&stopButton);
    stopButton.setButtonText("Stop");
    stopButton.onClick = [this] {stopButtonClicked();};
    stopButton.setColour(TextButton::buttonColourId, Colours::red);
    stopButton.setEnabled(false);
    
    
    
    setAudioChannels (2, 2);

}

MainComponent::~MainComponent()
{

    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    if(readerSource.get() == nullptr) {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    
    transportSource.getNextAudioBlock(bufferToFill);
    
}

void MainComponent::releaseResources()
{
    transportSource.releaseResources();

}

//==============================================================================

void MainComponent::changeState(TransportState newState)
{
    // check if state has been changed
    if(state != newState)
    {
        state = newState;
        switch(state)
        {
            case Stopped:
                stopButton.setEnabled(false);
                playButton.setEnabled(true);
                transportSource.setPosition(0.0);
                break;
            case Starting:
                playButton.setEnabled(false);
                transportSource.start(); // starts playing
                break;
            case Playing:
                stopButton.setEnabled(true);
                break;
                
            case Stopping:
                transportSource.stop();
                break;
                
        }
        
    }
}

void MainComponent::openButtonClicked()
{
        FileChooser chooser ("Select a Wave file to play... ",
                             {},
                             "*.wav");
        // popup
        if (chooser.browseForFileToOpen())
        {
            
            // if browseFor.. succeded we can return a file
            File file = chooser.getResult();
            
            // creating a pointer to reader for this particular file (AudioFormatReader)
            auto* reader = formatManager.createReaderFor(file);
            
            // we can proceed if reader is not null
            if(reader != nullptr)
            {
                // create a new reader source, since we have just opened
                // a new file for reading
                
                // note, AudioFormatReaderSource has only one constructor
                // that requires 2 args: pointer to AudioFormatReader object
                // and a bool deleteReaderWhenThisIsDeleted
                
                std::unique_ptr<AudioFormatReaderSource> newSource (new AudioFormatReaderSource (reader, true));
                
                // get() to get a pointer from unique_ptr
                transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
                playButton.setEnabled(true);
                
                // transfering ownership from newSource to readerSource
                readerSource.reset(newSource.release());
                
                
            }
            
        }
        
        
}

void MainComponent::playButtonClicked()
{
    changeState(Starting);
}

void MainComponent::stopButtonClicked()
{
    changeState(Stopping);
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    openButton.setBounds(10, 10, getWidth()-20, 20);
    playButton.setBounds(10, 40, getWidth()-20, 20);
    stopButton.setBounds(10, 70, getWidth()-20, 20);
 
}

void MainComponent::changeListenerCallback(ChangeBroadcaster* source)
{
    if(source == &transportSource)
    {
        if(transportSource.isPlaying())
            changeState(Playing);
        else
            changeState(Stopped);
            }
}
