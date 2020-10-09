/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BasicSamplerAudioProcessorEditor::BasicSamplerAudioProcessorEditor (BasicSamplerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    xy = std::make_pair(10, 4);

    bottomBar = std::make_unique<BottomBar> (p);
    addAndMakeVisible(&openButton);
    openButton.onClick = [&]()
    {
        audioProcessor.openButtonClicked();
        repaint();
    };
    //setResizable(true, true);
    //setResizeLimits(segmentLength * 5 / 2, segmentLength, segmentLength * 20, segmentLength * 8);
    setSize (xy.first * segmentLength, xy.second * segmentLength);
}

BasicSamplerAudioProcessorEditor::~BasicSamplerAudioProcessorEditor()
{
}

//==============================================================================
void BasicSamplerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);

    
}

void BasicSamplerAudioProcessorEditor::resized()
{

}

//==============================================================================
bool BasicSamplerAudioProcessorEditor::isInterestedInFileDrag(const juce::StringArray& files)
{
    for (juce::File filePath : files)
    {
        if (filePath.getFileExtension() == ".wav" || filePath.getFileExtension() == ".aiff")
        {
            return true;
        }
    }
    
    return false;
}

void BasicSamplerAudioProcessorEditor::filesDropped(const juce::StringArray &files, int x, int y)
{
    for (juce::File file : files)
    {
        if (this->isInterestedInFileDrag(files))
        {
            audioProcessor.setFilePath(file);
            audioProcessor.loadFile();
        }
    }
    
    repaint();
}

int BasicSamplerAudioProcessorEditor::count{0};
