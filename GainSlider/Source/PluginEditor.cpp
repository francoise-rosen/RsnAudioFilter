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
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    addAndMakeVisible(&gainSlider);
    gainSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    gainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 40);
    gainSlider.setSkewFactor(0.5);
    gainSlider.setRange(0.0, 1.0);
    gainSlider.setValue(0.25);
    gainSlider.addListener(this);
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
    gainSlider.setBounds(getLocalBounds());
}

void GainSliderAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == &gainSlider)
    {
        processor.setGain(gainSlider.getValue());
    }
}
