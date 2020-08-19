/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class FilterDesignAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FilterDesignAudioProcessorEditor (FilterDesignAudioProcessor&);
    ~FilterDesignAudioProcessorEditor() override;


    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
   
    FilterDesignAudioProcessor& audioProcessor;
    
    juce::Slider freqSlider, gainSlider, qSlider, dryWetSlider;
    juce::ComboBox filterType, order;
    juce::TextButton bypassButton;
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    
    std::unique_ptr<SliderAttachment> freqSliderAttachment;
    std::unique_ptr<SliderAttachment> qSliderAttachment;
    std::unique_ptr<SliderAttachment> dryWetSliderAttachment;
    std::unique_ptr<SliderAttachment> gainSliderAttachment;
    std::unique_ptr<ComboBoxAttachment> filterTypeBoxAttachment;
    std::unique_ptr<ComboBoxAttachment> orderBoxAttachment; 
    std::unique_ptr<ButtonAttachment> bypassButtonAttachment;
    
    int guiTextWidth{65};
    int guiTextHeight{25};
    
    void fillFilterTypeBox(const int& numOfTypes);
    void fillOrderBox(const int& numOfOrders);
    
    int edge{5};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterDesignAudioProcessorEditor)
};
