/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
StereoDelayProcessorEditor::StereoDelayProcessorEditor (StereoDelayProcessor& p)
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
    
    gainLabel.setText(StereoDelayProcessor::paramGain, dontSendNotification);
    gainLabel.attachToComponent(&gainSlider, false);
    
    delayLabelLeft.setText(StereoDelayProcessor::paramDelayLeft, dontSendNotification);
    delayLabelLeft.attachToComponent(&delaySliderLeft, false);
    
    delayLabelRight.setText(StereoDelayProcessor::paramDelayRight, dontSendNotification);
    delayLabelRight.attachToComponent(&delaySliderRight, false);
    
    fbLabel.setText(StereoDelayProcessor::paramFeedback, dontSendNotification);
    fbLabel.attachToComponent(&feedbackSlider, false);
    
    typeLabel.setText(StereoDelayProcessor::paramType, dontSendNotification);
    typeLabel.attachToComponent(&delayTypeBox, false);
    

    // ATTACHMENTS
    gainSliderAttachment = std::make_unique<SliderAttachment>(processor.accessTreeState(), StereoDelayProcessor::paramGain, gainSlider);
    delaySliderLeftAttachment = std::make_unique<SliderAttachment>(processor.accessTreeState(), StereoDelayProcessor::paramDelayLeft, delaySliderLeft);
    delaySliderRightAttachment = std::make_unique<SliderAttachment>(processor.accessTreeState(), StereoDelayProcessor::paramDelayRight, delaySliderRight);
    freedBackSliderAttachment = std::make_unique<SliderAttachment>(processor.accessTreeState(),
        StereoDelayProcessor::paramFeedback, feedbackSlider);
    typeAttachment = std::make_unique<ComboBoxAttachment>(processor.accessTreeState(), StereoDelayProcessor::paramType, delayTypeBox);
    

}

StereoDelayProcessorEditor::~StereoDelayProcessorEditor()
{
}

void StereoDelayProcessorEditor::fillTypeBox()
{
    int boxSize = StereoDelayProcessor::delayTypes.size();
    if (!boxSize) return;
    for (int i = 0; i < boxSize; ++i)
    {
        delayTypeBox.addItem(StereoDelayProcessor::delayTypes[i], 100 + i);
    }
}

//==============================================================================
void StereoDelayProcessorEditor::paint (Graphics& g)
{

    g.fillAll (Colours::mediumvioletred.withMultipliedBrightness(0.5));

    g.setColour (Colours::cyan);
    g.setFont (21.0f);
    
    g.drawText("rosen::delay", 0, 0, getWidth()/3, getHeight()/6, Justification::centred);

    
}

void StereoDelayProcessorEditor::resized()
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
