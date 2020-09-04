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
class BasicSamplerAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                          public juce::FileDragAndDropTarget
{
public:
    BasicSamplerAudioProcessorEditor (BasicSamplerAudioProcessor&);
    ~BasicSamplerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    //==============================================================================
    bool isInterestedInFileDrag (const juce::StringArray& files) override;
    void filesDropped (const juce::StringArray& files, int x, int y) override;
    

private:
    BasicSamplerAudioProcessor& audioProcessor;
    
    // GUI
    juce::TextButton openButton{"Open"};
    enum Frame {screenFrame, leftBar, rightBar, topBar, bottomBar, numOfFrames};
    std::vector<std::unique_ptr<juce::Rectangle<int>>> frames;
    void setFrames();
    std::vector<float> waveformPoints;
 
    // Window
    const int segmentLength{70};
    std::pair<int, int> xy;
    const int edge{5};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicSamplerAudioProcessorEditor)
};

//#if JUCE_DEBUG
//
//Logger::writeToLog("Debug Mode");
//
//#else
//
//Logger::writeToLog("Release Mode");
//
//#endif
