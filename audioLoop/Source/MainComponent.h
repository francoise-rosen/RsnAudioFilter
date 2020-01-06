/*
  ==============================================================================

  1. Add Label with error on the screen: "File is too long, please try reading a shorter one"
  2. Gain Slider
  3. Pitch Dial
  4. Stereo Bouncing for monophonic file
  5. Reverse playing

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
    
    void openButtonClicked();
    void clearButtonClicked();
    
    TextButton openButton;
    TextButton clearButton;
    
    AudioFormatManager formatManager; 
    AudioSampleBuffer playbackBuffer;
    int position;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
