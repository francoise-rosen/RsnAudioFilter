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
    : AudioProcessorEditor (&p), processor (p),
    level(processor.getGain()) // upon startup read the value off the processor?
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    addAndMakeVisible(&gainSliderdB);
    gainSliderdB.setSliderStyle(Slider::SliderStyle::LinearVertical);
    gainSliderdB.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 40);
    gainSliderdB.setRange(-100.0, 0.0);
    // you must set skew factor after the range is defined!
    gainSliderdB.setSkewFactorFromMidPoint(-16.0);
    gainSliderdB.setValue(Decibels::gainToDecibels((float)level));
    gainSliderdB.addListener(this);
    
    addAndMakeVisible(&dBLabel);
    dBLabel.setText("GAIN", dontSendNotification);
    dBLabel.attachToComponent(&gainSliderdB, false);
    
    // tree state
    sliderAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.accessTreeState(), GAIN_ID, gainSliderdB));
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
    gainSliderdB.setBounds(getLocalBounds().removeFromRight(70).removeFromBottom(220));
}

void GainSliderAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == &gainSliderdB)
    {
        // convert db to magnitude
        level = Decibels::decibelsToGain(gainSliderdB.getValue());
        processor.setGain(level);
    }
}
