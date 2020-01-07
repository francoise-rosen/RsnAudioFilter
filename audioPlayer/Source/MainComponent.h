/*
  ==============================================================================

   We want to read a file
 
 1. 

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent,
                        public ChangeListener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    
    // AudioSource methods:
    // AudioSource has to states: prepared and unprepared
    
    // prepareToPlay sets state to prepared
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    // then repeated calls will be made it its getNextAudioBlock
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    // and this will set the state to unprepared
    void releaseResources() override;
    


    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;
    
    // ChangeListener pure virtual function
    void changeListenerCallback(ChangeBroadcaster* source) override;

private:
    //==============================================================================
    // Your private member variables go here...
    
    // states are private
    
    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Stopping
    };
    
    // change state of reading from AudioFormatReaderSource
    void changeState(TransportState newState);
    
    // here we open a file
    void openButtonClicked();
    void playButtonClicked();
    void stopButtonClicked();
    
    // we change the state of playback here
        
    TextButton openButton;
    TextButton playButton;
    TextButton stopButton;
    
    // AudioFormatManager constains list of formats and creates objects for reading audio data from these formats
    AudioFormatManager formatManager;
    
    // when AudioFormatManager is asked to open a file it creates an instance of
    // AudioFormatReader (allows us to read the file in a consistent format (arrays of
    // floats or doubles
    
    // AudioFOrmatReaderSource reads data from AudioFormatReader and renders Audio
    // via its getNextAudioBlock() function
    std::unique_ptr<AudioFormatReaderSource> readerSource; // where do I use this?
    
    AudioTransportSource transportSource;
    TransportState state;
    
    


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
