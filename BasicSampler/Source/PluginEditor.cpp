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

    addAndMakeVisible(&openButton);
    openButton.onClick = [&]()
    {
        audioProcessor.openButtonClicked();
        repaint();
    };
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
    setFrames();

    g.setColour (juce::Colours::red);
    g.setFont (15.0f);
    g.drawFittedText("Sampler", 10, 10, 50, 30, juce::Justification::centred, 1);
    g.setColour(juce::Colours::darkgoldenrod);
    //g.drawRoundedRectangle(getLocalBounds().reduced(edge).toFloat(), 3, 2);
    
    //g.drawText(audioProcessor.getFileText(), getBounds().withTop(edge + 40).reduced(edge + edge), juce::Justification::bottomLeft);
    g.drawText(audioProcessor.getFileText(), frames[screenFrame]->reduced(edge), juce::Justification::bottomLeft);
    
    for (int i = 0; i < frames.size(); ++i)
    {
        g.setColour(juce::Colours::crimson);
        g.drawRoundedRectangle(frames[i]->toFloat(), 7, 2);
    }
    
    if (audioProcessor.filePathChanged)
    {
        // make a vector of samples
        waveformPoints.clear();
        auto xRatio = audioProcessor.getWaveform().getNumSamples() / frames[screenFrame]->getWidth();
        auto buffer = audioProcessor.getWaveform().getReadPointer(0);
        for (int i = 0; i < audioProcessor.getWaveform().getNumSamples(); i += xRatio)
        {
            waveformPoints.push_back(buffer[i]);
            DBG(buffer[i]);
        }
    
        // draw a waveform
        juce::Path p;
        p.startNewSubPath(static_cast<float>(frames[screenFrame]->getX()), static_cast<float>(frames[screenFrame]->getCentreY()));
        
        DBG("Printing x: ");
        DBG(frames[screenFrame]->getX());
    
        for (int pointX = 0; pointX < waveformPoints.size(); ++pointX)
        {
            auto pointY = juce::jmap(waveformPoints[pointX], -1.0f, 1.0f, static_cast<float>(frames[screenFrame]->getBottom()), static_cast<float>(frames[screenFrame]->getY()));
            p.lineTo(pointX, pointY);
        }
        g.setColour(juce::Colours::olivedrab);
        g.strokePath(p, juce::PathStrokeType(2));
        
        audioProcessor.filePathChanged = false;
    }
    
}

void BasicSamplerAudioProcessorEditor::resized()
{
    setFrames();
    auto buttonArea = getLocalBounds();
    openButton.setBounds(buttonArea.removeFromTop(getHeight()/4).removeFromRight(getWidth()/5).reduced(10));
    
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

void BasicSamplerAudioProcessorEditor::setFrames()
{
    frames.resize((int)Frame::numOfFrames);
    auto horizontalBarHeight = getHeight() / 6;
    auto leftBarWidth = getWidth() / 10;
    auto rightBarWidth = getWidth() / 5;
    auto area = getLocalBounds();
    frames[leftBar] = std::make_unique<juce::Rectangle<int>>(area.removeFromLeft(leftBarWidth));
    frames[rightBar] = std::make_unique<juce::Rectangle<int>>(area.removeFromRight(rightBarWidth));
    frames[topBar] = std::make_unique<juce::Rectangle<int>>(area.removeFromTop(horizontalBarHeight));
    frames[bottomBar] = std::make_unique<juce::Rectangle<int>>(area.removeFromBottom(horizontalBarHeight));
    frames[screenFrame] = std::make_unique<juce::Rectangle<int>>(area);
    
    for (int i = 0; i < frames.size(); ++i)
    {
        assert(frames[i] != nullptr);
    }
}
