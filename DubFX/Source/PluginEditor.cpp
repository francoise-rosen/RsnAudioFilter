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
    
    addAndMakeVisible(&delayTypeBox);
    fillTypeBox();
    
    gainLabel.setText(DubFxAudioProcessor::paramGain, dontSendNotification);
    gainLabel.attachToComponent(&gainSlider, false);
    
    delayLabelLeft.setText(DubFxAudioProcessor::paramDelayLeft, dontSendNotification);
    delayLabelLeft.attachToComponent(&delaySliderLeft, false);
    
    delayLabelRight.setText(DubFxAudioProcessor::paramDelayRight, dontSendNotification);
    delayLabelRight.attachToComponent(&delaySliderRight, false);
    
    fbLabel.setText(DubFxAudioProcessor::paramFeedback, dontSendNotification);
    fbLabel.attachToComponent(&feedbackSlider, false);
    
    typeLabel.setText(DubFxAudioProcessor::paramType, dontSendNotification);
    typeLabel.attachToComponent(&delayTypeBox, false);
    

    // ATTACHMENTS
    gainSliderAttachment = std::make_unique<SliderAttachment>(processor.accessTreeState(), DubFxAudioProcessor::paramGain, gainSlider);
    delaySliderLeftAttachment = std::make_unique<SliderAttachment>(processor.accessTreeState(), DubFxAudioProcessor::paramDelayLeft, delaySliderLeft);
    delaySliderRightAttachment = std::make_unique<SliderAttachment>(processor.accessTreeState(), DubFxAudioProcessor::paramDelayRight, delaySliderRight);
    freedBackSliderAttachment = std::make_unique<SliderAttachment>(processor.accessTreeState(),
        DubFxAudioProcessor::paramFeedback, feedbackSlider);
    typeAttachment = std::make_unique<ComboBoxAttachment>(processor.accessTreeState(), DubFxAudioProcessor::paramType, delayTypeBox);
    

}

DubFxAudioProcessorEditor::~DubFxAudioProcessorEditor()
{
}

void DubFxAudioProcessorEditor::fillTypeBox()
{
    int boxSize = DubFxAudioProcessor::delayTypes.size();
    if (!boxSize) return;
    for (int i = 0; i < boxSize; ++i)
    {
        delayTypeBox.addItem(DubFxAudioProcessor::delayTypes[i], 100 + i);
    }
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
    auto gainArea = box.removeFromRight(dialWidth);
    
    gainSlider.setBounds(gainArea.removeFromBottom(getHeight()/2).reduced(edgeThickness));
    delayTypeBox.setBounds(gainArea.reduced(edgeThickness));
    delaySliderLeft.setBounds(delaySlidersBox.removeFromTop(box.getHeight() / 2).reduced(edgeThickness));
    delaySliderRight.setBounds(delaySlidersBox.reduced(edgeThickness));
    feedbackSlider.setBounds(box.reduced(edgeThickness));
    

}
