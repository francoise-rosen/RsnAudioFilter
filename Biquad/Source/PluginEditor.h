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
class BiquadAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    BiquadAudioProcessorEditor (BiquadAudioProcessor&);
    ~BiquadAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    BiquadAudioProcessor& audioProcessor;
    
    // Gui
    juce::Slider freqSlider;
    juce::Slider qSlider;
    juce::Slider dryWetSlider;
    juce::Slider gainSlider;
    juce::ComboBox typeBox;
    juce::ComboBox orderBox;
    juce::TextButton bypassButton;
    
    // Attachments
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    
    std::unique_ptr<SliderAttachment> freqSliderAttachment;
    std::unique_ptr<SliderAttachment> qSliderAttachment;
    std::unique_ptr<SliderAttachment> dryWetSliderAttachment;
    std::unique_ptr<SliderAttachment> gainSliderAttachment;
    std::unique_ptr<ComboBoxAttachment> typeBoxAttachment;
    std::unique_ptr<ComboBoxAttachment> orderBoxAttachment;
    std::unique_ptr<ButtonAttachment> bypassButtonAttachment;
    
    int guiTextWidth{65};
    int guiTextHeight{25};
    
    void fillTypeBox(int numOfTypes);
    void fillOrderBox(const int& order);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BiquadAudioProcessorEditor)
};
