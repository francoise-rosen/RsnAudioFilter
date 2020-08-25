/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ResonAudioProcessorEditor::ResonAudioProcessorEditor (ResonAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    
    freqSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    freqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textboxWidth, textboxHeight);
    addAndMakeVisible(&freqSlider);
    
    qSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    qSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, textboxWidth, textboxHeight);
    addAndMakeVisible(&qSlider);
    
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textboxWidth, textboxHeight);
    addAndMakeVisible(&gainSlider);
    
    addAndMakeVisible(&algoBox);
    fillAlgoBox();
    
    bypassButton.setButtonText("On");
    bypassButton.setToggleState(true, juce::dontSendNotification);
    addAndMakeVisible(&bypassButton);
    
    // construct an object for each attachment
    freqSliderAttachment = std::make_unique<SliderAttachment>(audioProcessor.getValueTree(), audioProcessor.paramFreq, freqSlider);
    qSliderAttachment = std::make_unique<SliderAttachment>(audioProcessor.getValueTree(), audioProcessor.paramQ, qSlider);
    gainSliderAttachment = std::make_unique<SliderAttachment>(audioProcessor.getValueTree(), audioProcessor.paramGain, gainSlider);
    algoBoxAttachment = std::make_unique<ComboBoxAttachment>(audioProcessor.getValueTree(), audioProcessor.paramAlgorithm, algoBox);
    bypassAttachment = std::make_unique<ButtonAttachment>(audioProcessor.getValueTree(), audioProcessor.paramBypass, bypassButton);
    
    setSize (340, 270);
}

ResonAudioProcessorEditor::~ResonAudioProcessorEditor()
{
}

//==============================================================================

void ResonAudioProcessorEditor::fillAlgoBox()
{
    // so far just 2 algorithms
    // size of enum matches size of array names
    assert (ResonAudioProcessor::filterAlgorithms.size() == (int)sfd::FilterAlgorithm::numOfAlgorithms);
    for (int i = 0; i < (int)sfd::FilterAlgorithm::numOfAlgorithms; ++i)
    {
        algoBox.addItem(ResonAudioProcessor::filterAlgorithms[i], 100 + i);
    }
}

//==============================================================================
void ResonAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
  
}

void ResonAudioProcessorEditor::resized()
{
    auto filterArea = getLocalBounds().removeFromLeft(getWidth()/2);
    auto gainArea = getLocalBounds().removeFromRight(getWidth()/2);
    int barHeight = 150;
    auto edge = 10;
    
    qSlider.setBounds(filterArea.removeFromBottom(getHeight() - barHeight).reduced(edge));
    freqSlider.setBounds(filterArea.reduced(filterArea.getWidth()/4, edge));
    
    algoBox.setBounds(gainArea.removeFromTop(getHeight()/4).reduced(edge, edge * 2));
    bypassButton.setBounds(gainArea.removeFromTop(juce::jmin(getWidth()/4, gainArea.getHeight()/3)).reduced(edge, juce::jmin(edge * 2, gainArea.getHeight()/5)));
    
    gainSlider.setBounds(gainArea.reduced(edge));

    
//    freqSlider.setBounds(filterArea.removeFromTop(sliderHeight).reduced(edge));
//    qSlider.setBounds(filterArea.reduced(edge));
    
    
    
}


