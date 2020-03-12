/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GainSliderAudioProcessorEditor::GainSliderAudioProcessorEditor (GainSliderAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
    // upon startup read the value off the processor?
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    addAndMakeVisible(&gainSlider);
    gainSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    gainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 40);
   gainSlider.setRange(0.0, 1.0);
    // you must set skew factor after the range is defined!
    gainSlider.setSkewFactorFromMidPoint(-16.0);
    gainSlider.setValue(Decibels::gainToDecibels((float)level));
    //gainSlider.addListener(this);
    
    addAndMakeVisible(&dBLabel);
    dBLabel.setText("GAIN", dontSendNotification);
    dBLabel.attachToComponent(&gainSlider, false);
    
    // tree state
    sliderAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.accessTreeState(), GAIN_ID, gainSlider));
}

GainSliderAudioProcessorEditor::~GainSliderAudioProcessorEditor()
{
}

//==============================================================================
void GainSliderAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

}

void GainSliderAudioProcessorEditor::resized()
{
    gainSlider.setBounds(getLocalBounds().removeFromRight(70).removeFromBottom(220));
}

