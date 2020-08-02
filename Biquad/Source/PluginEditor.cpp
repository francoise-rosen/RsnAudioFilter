/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
BiquadAudioProcessorEditor::BiquadAudioProcessorEditor (BiquadAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // set up the gui
    // SLIDERS
    freqSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    freqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, guiTextWidth, guiTextHeight);
    addAndMakeVisible(&freqSlider);
    
    qSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    qSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, guiTextWidth, guiTextHeight);
    addAndMakeVisible(&qSlider);
    
    dryWetSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    dryWetSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, guiTextWidth, guiTextHeight);
    addAndMakeVisible(&dryWetSlider);
    
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, guiTextWidth, guiTextHeight);
    addAndMakeVisible(&gainSlider);
    
    // ComboBoxes
    addAndMakeVisible(&typeBox);
    addAndMakeVisible(&orderBox);
    fillTypeBox(2); // test with just HPF and LPF
    fillOrderBox(2); // here just 1 and 2 order filters
    
    // button
    bypassButton.setName("Bypass");
    bypassButton.setButtonText("ON/OFF");
    bypassButton.setClickingTogglesState(true);
    addAndMakeVisible(&bypassButton);
    
    // attach the gui to the AudioProcessor
    freqSliderAttachment = std::make_unique<SliderAttachment>(audioProcessor.getValueTree(), audioProcessor.freqParam, freqSlider);
    qSliderAttachment = std::make_unique<SliderAttachment>(audioProcessor.getValueTree(), audioProcessor.qParam, qSlider);
    dryWetSliderAttachment = std::make_unique<SliderAttachment>(audioProcessor.getValueTree(), audioProcessor.dryWetParam, dryWetSlider);
    gainSliderAttachment = std::make_unique<SliderAttachment>(audioProcessor.getValueTree(), audioProcessor.gainParam, gainSlider);
    typeBoxAttachment = std::make_unique<ComboBoxAttachment>(audioProcessor.getValueTree(), audioProcessor.typeParam, typeBox);
    orderBoxAttachment = std::make_unique<ComboBoxAttachment>(audioProcessor.getValueTree(), audioProcessor.orderParam, orderBox);
    bypassButtonAttachment = std::make_unique<ButtonAttachment>(audioProcessor.getValueTree(), audioProcessor.bypassParam, bypassButton);
    
    setSize (300, 420);
}

BiquadAudioProcessorEditor::~BiquadAudioProcessorEditor()
{
}

//==============================================================================
// fill the combos

void BiquadAudioProcessorEditor::fillTypeBox(int numOfTypes)
{
    assert (numOfTypes > 1);
    for (int i = 0; i < numOfTypes; ++i)
    {
        if (i < audioProcessor.filterTypeList.size()) typeBox.addItem(audioProcessor.filterTypeList[i], 100 + i);
        else
            typeBox.addItem("FilterType " + juce::String(i), 100 + i);
    }
}

void BiquadAudioProcessorEditor::fillOrderBox(const int &order)
{
    if (order < 1) return;
    for (int i = 0; i < order; ++i)
    {
        orderBox.addItem(juce::String(audioProcessor.orderRollOff[i]), 100 + i);
    }
}

//==============================================================================
void BiquadAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawText("Rosen::Biquad", 10, 10, getWidth()/3, 30, juce::Justification::centred);

}

void BiquadAudioProcessorEditor::resized()
{
   
    // freq slider, q slider, wet/dry, gain, type box, bypass button, order box (prints rolloff in Hz)
    
    auto filterArea = getLocalBounds().removeFromLeft(getWidth()/2);
    auto gainArea = getLocalBounds().removeFromRight(getWidth()/2);
    typeBox.setBounds(filterArea.removeFromBottom(filterArea.getHeight() / 4).reduced(edge, edge * 7));
    qSlider.setBounds(filterArea.removeFromBottom(filterArea.getHeight() / 3).reduced(edge));
    freqSlider.setBounds(filterArea.removeFromBottom(filterArea.getHeight() * 3/4).reduced(edge * 2));
    
    gainSlider.setBounds(gainArea.removeFromBottom(gainArea.getHeight() * 3 / 8).reduced(edge * 4));
    dryWetSlider.setBounds(gainArea.removeFromBottom(getHeight() * 3 / 8).reduced(edge * 4));
    orderBox.setBounds(gainArea.removeFromLeft(gainArea.getWidth()/2).reduced(edge * 2, edge * 7));
    bypassButton.setBounds(gainArea.reduced(edge));
    
    
    
}
