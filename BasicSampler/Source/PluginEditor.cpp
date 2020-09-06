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
    setFrames();

    g.setColour (juce::Colours::gold);
    g.setFont (15.0f);
    g.drawFittedText("Sampler", 10, 10, 50, 30, juce::Justification::centred, 1);
    g.drawLine(frames[screenFrame]->getX(), frames[screenFrame]->getCentreY(), frames[screenFrame]->getRight(), frames[screenFrame]->getCentreY());
    g.setColour(juce::Colours::darkgoldenrod);

    g.drawText(audioProcessor.getFileText(), frames[screenFrame]->reduced(edge), juce::Justification::bottomLeft);
    
    for (int i = 0; i < frames.size(); ++i)
    {
        g.setColour(juce::Colours::crimson);
        g.drawRoundedRectangle(frames[i]->toFloat(), 7, 2);
    }
    
    g.setColour(juce::Colours::orange);

    
    // MAKE SCREEN A CHILD with its own paint() ?
    if (audioProcessor.filePathChanged)
    {
        // make a vector of samples
        audioProcessor.filePathChanged = false;
        waveformPoints.clear();
        auto numSamples = audioProcessor.getWaveform().getNumSamples();
        auto xRatio = numSamples / (frames[screenFrame]->getWidth() - 3 * edge);
        auto buffer = audioProcessor.getWaveform().getReadPointer(0);
        for (int i = 0; i < numSamples; i += xRatio)
        {
            waveformPoints.push_back(buffer[i]);
            DBG(buffer[i]); // does not work!
        }
    
        // draw a waveform
        juce::Path p;
        p.startNewSubPath(static_cast<float>(frames[screenFrame]->getX()) + edge, static_cast<float>(frames[screenFrame]->getCentreY()));
        
        DBG("Printing x: ");
        DBG(frames[screenFrame]->getX());
        
        int pointX = frames[screenFrame]->getX() + 1 + edge;
        
//        g.drawFittedText("Screen width: " + juce::String(frames[screenFrame]->getWidth()), *frames[topBar] , juce::Justification::centred, 1);
        
        g.drawFittedText("Number of samples: " + juce::String(waveformPoints.size()), *frames[bottomBar], juce::Justification::centred, 1);
        
        for (int i = 0; i < waveformPoints.size(); ++i)
        {
            auto pointY = juce::jmap(waveformPoints[i], -1.0f, 1.0f, static_cast<float>(frames[screenFrame]->getBottom()), static_cast<float>(frames[screenFrame]->getY()));
            ++pointX;
            p.lineTo(pointX, pointY);
        }
        g.setColour(juce::Colours::olivedrab);
        g.strokePath(p, juce::PathStrokeType(2));
        
        return;
        
    }

    
}

void BasicSamplerAudioProcessorEditor::resized()
{
    setFrames();
    openButton.setBounds(frames[rightBar]->removeFromTop(getHeight()/6).reduced(edge));
   // audioProcessor.filePathChanged = true; // does not work! We may want to draw on a child
    // and resize that one?
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
    frames.clear();
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

int BasicSamplerAudioProcessorEditor::count{0};
