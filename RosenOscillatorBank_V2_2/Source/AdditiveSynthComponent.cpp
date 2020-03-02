/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "AdditiveSynthComponent.h"

//==============================================================================
AdditiveSynthComponent::AdditiveSynthComponent()
{
    setSize (400, 300);
    
    // GUI
    addAndMakeVisible(&frequencySlider);
    frequencySlider.setRange(30, 5000);
    frequencySlider.setSkewFactorFromMidPoint(440);
    frequencySlider.setValue(440);
    frequencySlider.setSliderStyle(Slider::SliderStyle::Rotary);
    frequencySlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 100, 20);
    frequencySlider.addListener(this);
    
    frequencyLabel.setText("Freq", dontSendNotification);
    frequencyLabel.attachToComponent(&frequencySlider, false);
    addAndMakeVisible(&frequencySlider);
    
    addAndMakeVisible(&octaveSelector);
    octaveSelector.setRange(octaveRange, 1);
    octaveSelector.setValue(0);
    octaveSelector.setSliderStyle(Slider::SliderStyle::Rotary);
    octaveSelector.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 100, 20);
    octaveSelector.addListener(this);
    
    addAndMakeVisible(&magnitudeSlider);
    magnitudeSlider.setRange(0.0, 1.0);
    magnitudeSlider.setValue(0.0);
    magnitudeSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
    magnitudeSlider.addListener(this);


    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }
    else
    {

        setAudioChannels (2, 2);
    }
}

AdditiveSynthComponent::~AdditiveSynthComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void AdditiveSynthComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    
    // single oscillator allocation. works!
//    frequency = frequencySlider.getValue();
//    magnitude = magnitudeSlider.getValue();
//    phasor.reset(new Phasor(frequency, sampleRate));
//    sineOscillator.reset(new SineOSC(phasor.get()));
    
    //=========================================================================
    
// using constructor with GUI
    
    sineOscillator.reset(new SineOSC());

}

void AdditiveSynthComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    auto* leftOutput = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* rightOutput = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
    
    gain = 0.25;

    bufferToFill.clearActiveBufferRegion();
    
    phasor.get()->updateFrequency(frequency);
    sineOscillator.get()->setMagnitude(magnitude);
    
    // access the phasor via oscillator
    //sineOscillator->accessPhasor().updateFrequency(frequency);
    
    for( int sample = 0; sample < bufferToFill.numSamples; ++sample)
    {
        auto sampleValue = sineOscillator->getNextSample();
        leftOutput[sample] = sampleValue * gain;
        rightOutput[sample] = sampleValue * gain;
    }
}

void AdditiveSynthComponent::releaseResources()
{

}

//==============================================================================

void AdditiveSynthComponent::sliderValueChanged(Slider* slider)
{
    if (slider == &frequencySlider)
        frequency = frequencySlider.getValue();
    if (slider == &magnitudeSlider)
        magnitude = magnitudeSlider.getValue();
}

//==============================================================================
void AdditiveSynthComponent::paint (Graphics& g)
{
 
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    
}

void AdditiveSynthComponent::resized()
{
    auto rotarySliderWidth = 100;
    auto spacer = 10;
    
    frequencySlider.setBounds(spacer, spacer + spacer, rotarySliderWidth, rotarySliderWidth);
    magnitudeSlider.setBounds(rotarySliderWidth + spacer, spacer  + spacer, 20, rotarySliderWidth);
    octaveSelector.setBounds(spacer, getHeight()/2, rotarySliderWidth, rotarySliderWidth);
   
}
