/*
  ==============================================================================

    LFOSection.cpp
    Created: 20 Sep 2020 12:27:44pm
    Author:  syfo_dias

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LFOSection.h"

//==============================================================================
LFOSection::LFOSection()
{

}

LFOSection::~LFOSection()
{
}

void LFOSection::paint (juce::Graphics& g)
{


    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("LFOSection", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
    
    Arrow<float> arrow;
    arrow.setLinePoints ({getWidth() * 0.5f, getHeight() * 0.5f}, {getWidth() * 0.75f, getHeight() * 0.75f});
    arrow.draw (g, juce::Colours::red);
    
    juce::Path p;
    p.startNewSubPath(getWidth() * 0.5f, getHeight() * 0.5f);
    p.lineTo(getWidth() * 0.75f, getHeight() * 0.75f);
    p.closeSubPath();
    g.setColour(juce::Colours::red);
    g.strokePath(p, juce::PathStrokeType {3.0f});
}

void LFOSection::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
