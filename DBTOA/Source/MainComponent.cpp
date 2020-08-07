/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{

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
    
    dBGain.setTextValueSuffix("dB");
    dBGain.setSliderStyle(Slider::SliderStyle::LinearVertical);
    dBGain.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 30);
    dBGain.setRange(-200.0, 6.0);
    dBGain.setValue(0.0);
    dBGain.addListener(this);
    addAndMakeVisible(&dBGain);
    dBLabel.setText("Gain In DB", dontSendNotification);
    dBLabel.attachToComponent(&dBGain, false);
    
    gain.setTextValueSuffix("Linear");
    gain.setSliderStyle(Slider::SliderStyle::LinearVertical);
    gain.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 30);
    gain.setRange(0.0, 1.0);
    gain.setValue(1.0);
    gain.addListener(this);
    addAndMakeVisible(&gain);
    gainLabel.setText("Linear", dontSendNotification);
    gainLabel.attachToComponent(&gain, false);
    
    addAndMakeVisible(&gain_to_dB);
    addAndMakeVisible(&dB_to_gain);
    
    gain_to_dB.setColour(Label::backgroundColourId, Colours::blueviolet);
    gain_to_dB.setColour(Label::textColourId, Colours::black);
    
    dB_to_gain.setColour(Label::backgroundColourId, Colours::peachpuff);
    dB_to_gain.setColour(Label::textColourId, Colours::black);
    
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
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

void MainComponent::sliderValueChanged(Slider* slider)
{
    if (slider == &dBGain)
    {
        auto dbtoa = Decibels::decibelsToGain(dBGain.getValue());
        std::cout << "dbtoa: " << dbtoa << '\n';
        String message = String(dbtoa);
        Logger::getCurrentLogger()->writeToLog(message);
        dB_to_gain.setText(String(dbtoa), dontSendNotification);
 
    }
    
    else if (slider == &gain)
    {
        auto atodb = Decibels::gainToDecibels(gain.getValue());
        std::cout << "atodb: " << atodb << '\n';
        gain_to_dB.setText(String(atodb), dontSendNotification);

    }
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
    
    auto box = getLocalBounds().reduced(20);
    
    dBGain.setBounds(box.removeFromLeft(getWidth()/4).reduced(19));
    gain.setBounds(box.removeFromLeft(getWidth()/4).reduced(19));
    gain_to_dB.setBounds(box.removeFromTop(getHeight()/2).reduced(10));
    dB_to_gain.setBounds(box.reduced(10));

}
