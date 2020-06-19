/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DubFxAudioProcessorEditor::DubFxAudioProcessorEditor (DubFxAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setSize (500, 300);
    
    // SLIDERS
    gainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    gainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, textboxWidth, textboxHeight);
    addAndMakeVisible(&gainSlider);

    delaySliderLeft.setSliderStyle(Slider::SliderStyle::Rotary);
    delaySliderLeft.setTextBoxStyle(Slider::TextBoxBelow, false, textboxWidth, textboxHeight);
    addAndMakeVisible(&delaySliderLeft);
    
    delaySliderRight.setSliderStyle(Slider::SliderStyle::Rotary);
    delaySliderRight.setTextBoxStyle(Slider::TextBoxBelow, false, textboxWidth, textboxHeight);
    addAndMakeVisible(&delaySliderRight);
   
    feedbackSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    feedbackSlider.setTextBoxStyle(Slider::TextBoxBelow, false, textboxWidth, textboxHeight);
    addAndMakeVisible(&feedbackSlider);
    
    gainLabel.setText(DubFxAudioProcessor::paramGain, dontSendNotification);
    gainLabel.attachToComponent(&gainSlider, false);
    
    delayLabelLeft.setText(DubFxAudioProcessor::paramDelayLeft, dontSendNotification);
    delayLabelLeft.attachToComponent(&delaySliderLeft, false);
    
    delayLabelRight.setText(DubFxAudioProcessor::paramDelayRight, dontSendNotification);
    delayLabelRight.attachToComponent(&delaySliderRight, false);
    
    fbLabel.setText(DubFxAudioProcessor::paramFeedback, dontSendNotification);
    fbLabel.attachToComponent(&feedbackSlider, false);
    

    // ATTACHMENTS
    gainSliderAttachment = std::make_unique<SliderAttachment>(processor.accessTreeState(), DubFxAudioProcessor::paramGain, gainSlider);
    delaySliderLeftAttachment = std::make_unique<SliderAttachment>(processor.accessTreeState(), DubFxAudioProcessor::paramDelayLeft, delaySliderLeft);
    delaySliderRightAttachment = std::make_unique<SliderAttachment>(processor.accessTreeState(), DubFxAudioProcessor::paramDelayRight, delaySliderRight);
    freedBackSliderAttachment = std::make_unique<SliderAttachment>(processor.accessTreeState(),
        DubFxAudioProcessor::paramFeedback, feedbackSlider);
    

}

DubFxAudioProcessorEditor::~DubFxAudioProcessorEditor()
{
}

//==============================================================================
void DubFxAudioProcessorEditor::paint (Graphics& g)
{

    g.fillAll (Colours::mediumvioletred.withMultipliedBrightness(0.5));

    g.setColour (Colours::cyan);
    g.setFont (21.0f);
    
    g.drawText("rosen::delay", 0, 0, getWidth()/3, getHeight()/6, Justification::centred);

    
}

void DubFxAudioProcessorEditor::resized()
{
    auto box = getLocalBounds().reduced(edgeThickness);
    const float dialWidth = box.getWidth() / 3;
    
    auto delaySlidersBox = box.removeFromLeft(dialWidth);
    
    gainSlider.setBounds(box.removeFromRight(dialWidth).reduced(edgeThickness));
    delaySliderLeft.setBounds(delaySlidersBox.removeFromTop(box.getHeight() / 2).reduced(edgeThickness));
    delaySliderRight.setBounds(delaySlidersBox.reduced(edgeThickness));
    feedbackSlider.setBounds(box.reduced(edgeThickness));
    

}
