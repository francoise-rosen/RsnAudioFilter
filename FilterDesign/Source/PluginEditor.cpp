/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FilterDesignAudioProcessorEditor::FilterDesignAudioProcessorEditor (FilterDesignAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    
    freqSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    freqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, guiTextWidth, guiTextHeight);
    addAndMakeVisible(&freqSlider);
    
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, guiTextWidth, guiTextHeight);
    addAndMakeVisible(&gainSlider);
    
    qSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    qSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, guiTextWidth, guiTextHeight);
    addAndMakeVisible(&qSlider);
    
    dryWetSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    dryWetSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, guiTextWidth, guiTextHeight);
    addAndMakeVisible(&dryWetSlider);
    
    fillFilterTypeBox(4);
    addAndMakeVisible(&filterType);
    fillOrderBox(audioProcessor.filterRollOff.size());
    addAndMakeVisible(&order);
    
    bypassButton.setName("Bypass");
    bypassButton.setButtonText("ON/OFF");
    bypassButton.setClickingTogglesState(true);
    addAndMakeVisible(&bypassButton);
    
    
    
    // ATTACHMENTS
    // arguments : proc value tree, parameter's id, gui to attach
    freqSliderAttachment = std::make_unique<SliderAttachment>(audioProcessor.getValueTree(), audioProcessor.freqParam, freqSlider);
    qSliderAttachment = std::make_unique<SliderAttachment>(audioProcessor.getValueTree(), audioProcessor.qParam, qSlider);
    gainSliderAttachment = std::make_unique<SliderAttachment>(audioProcessor.getValueTree(), audioProcessor.gainParam, gainSlider);
    dryWetSliderAttachment = std::make_unique<SliderAttachment>(audioProcessor.getValueTree(), audioProcessor.dryWetParam, dryWetSlider);
    
    filterTypeBoxAttachment = std::make_unique<ComboBoxAttachment>(audioProcessor.getValueTree(), audioProcessor.filterTypeParam, filterType);
    orderBoxAttachment = std::make_unique<ComboBoxAttachment>(audioProcessor.getValueTree(), audioProcessor.orderParam, order);
    
    bypassButtonAttachment = std::make_unique<ButtonAttachment>(audioProcessor.getValueTree(), audioProcessor.bypassParam, bypassButton);
    
    setSize (220, 400);
}

FilterDesignAudioProcessorEditor::~FilterDesignAudioProcessorEditor()
{
}

//==============================================================================

void FilterDesignAudioProcessorEditor::fillFilterTypeBox(const int& numOfChoices)
{
    if (numOfChoices < 1) return;
    for (int i = 0; i < numOfChoices; ++i)
    {
        
        if (i < audioProcessor.algorithmIDs.size())
        {
            filterType.addItem(audioProcessor.algorithmIDs[i], 100 + i);
        }
        
        else
        {
            filterType.addItem("Filter " + juce::String(i), 100 + i);
        }
        
    }
}

void FilterDesignAudioProcessorEditor::fillOrderBox(const int& numOfOrders)
{
    assert (numOfOrders > 0);
    for (int i = 0; i < numOfOrders; ++i)
        order.addItem(audioProcessor.filterRollOff[i], i + 1);
}

//==============================================================================
void FilterDesignAudioProcessorEditor::paint (juce::Graphics& g)
{
 
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void FilterDesignAudioProcessorEditor::resized()
{
    auto gainArea = getBounds().removeFromRight(getWidth()/4.0);
    auto filterArea = getBounds().removeFromLeft(getWidth() * 3.0/4.0);
    
    bypassButton.setBounds(gainArea.removeFromTop(getHeight()/5.0).reduced(edge * 2));
    gainSlider.setBounds(gainArea.removeFromBottom(gainArea.getHeight()/3.0).reduced(edge));
    dryWetSlider.setBounds(gainArea);
    
    auto boxesArea = filterArea.removeFromTop(filterArea.getHeight()/4.0);
    filterType.setBounds(boxesArea.removeFromTop(boxesArea.getHeight()/2.0).reduced(edge));
    order.setBounds(boxesArea.reduced(edge));
    
    auto filterDialArea = filterArea.removeFromBottom(getHeight() - boxesArea.getHeight());
    freqSlider.setBounds(filterDialArea.removeFromBottom(filterDialArea.getHeight()/2).reduced(edge * 3));
    qSlider.setBounds(filterDialArea.reduced(edge * 3));
    
    
    
    
    
    
 
}
