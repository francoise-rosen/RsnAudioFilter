/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleDelayFeedbackAudioProcessorEditor::SimpleDelayFeedbackAudioProcessorEditor (SimpleDelayFeedbackAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 250);
    
    
    // GUI
    gainSlider.setName("Gain");
    gainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    gainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, textboxWidth, textboxHeight);
    addAndMakeVisible(&gainSlider);
    //gainSlider.setTextValueSuffix("dB");
    
    delaySlider.setName("Delay");
    delaySlider.setSliderStyle(Slider::SliderStyle::Rotary);
    delaySlider.setTextBoxStyle(Slider::TextBoxBelow, false, textboxWidth, textboxHeight);
    addAndMakeVisible(&delaySlider);
    //delaySlider.setTextValueSuffix("ms");
    
    feedbackSlider.setName("Feedback");
    feedbackSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    feedbackSlider.setTextBoxStyle(Slider::TextBoxBelow, false, textboxWidth, textboxHeight);
    addAndMakeVisible(&feedbackSlider);
    
    gainLabel.setText(Processor::paramGain, dontSendNotification);
    gainLabel.attachToComponent(&gainSlider, false);
    
    delayLabel.setText(Processor::paramDelay, dontSendNotification);
    delayLabel.attachToComponent(&delaySlider, false);
    
    fbLabel.setText(Processor::paramFeedback, dontSendNotification);
    fbLabel.attachToComponent(&feedbackSlider, false);
    
    

    // ATTACHMENTS
    gainSliderAttachment = std::make_unique<SliderAttachment>(processor.accessTreeState(), Processor::paramGain, gainSlider);
    delaySliderAttachment = std::make_unique<SliderAttachment>(processor.accessTreeState(), Processor::paramDelay, delaySlider);
    freedBackSliderAttachment = std::make_unique<SliderAttachment>(processor.accessTreeState(),
        Processor::paramFeedback, feedbackSlider);
    

}

SimpleDelayFeedbackAudioProcessorEditor::~SimpleDelayFeedbackAudioProcessorEditor()
{
}

//==============================================================================
void SimpleDelayFeedbackAudioProcessorEditor::paint (Graphics& g)
{

    g.fillAll (Colours::mediumvioletred.withMultipliedBrightness(0.5));

    g.setColour (Colours::cyan);
    g.setFont (21.0f);
    
    g.drawText("rosen::delay", 0, 0, getWidth()/3, getHeight()/6, Justification::centred);

    
}

void SimpleDelayFeedbackAudioProcessorEditor::resized()
{
    auto box = getLocalBounds().reduced(edgeThickness);
    const float dialWidth = box.getWidth() / 3;
    
    gainSlider.setBounds(box.removeFromRight(dialWidth).reduced(edgeThickness));
    delaySlider.setBounds(box.removeFromLeft(dialWidth).reduced(edgeThickness));
    feedbackSlider.setBounds(box.reduced(edgeThickness));
    

}
