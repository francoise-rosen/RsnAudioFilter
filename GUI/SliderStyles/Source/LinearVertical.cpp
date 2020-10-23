/*
  ==============================================================================

    LinearVertical.cpp
    Created: 21 Oct 2020 4:57:54pm
    Author:  syfo_dias

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LinearVertical.h"

//==============================================================================
LinearVertical::LinearVertical()
{
    addAndMakeVisible (&alphaVertical);
    addAndMakeVisible (&alphaVerticalSym);
    addAndMakeVisible (&alphaVerticalSymGradient);
    setLookAndFeel (&alphaOneLookAndFeel);
    alphaVerticalSym.setLookAndFeel (&alphaOneSymLF);
    alphaVerticalSymGradient.setLookAndFeel (&alphaOneSymGradientLF);
}

LinearVertical::~LinearVertical()
{
    setLookAndFeel (nullptr);
    alphaVerticalSym.setLookAndFeel (nullptr);
    alphaVerticalSymGradient.setLookAndFeel (nullptr);
}

void LinearVertical::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::purple.brighter());
    auto area = getLocalBounds().reduced (5.0f);
    g.setColour (juce::Colours::blue.darker());
    g.drawRect(area, 5.0f);

}

void LinearVertical::resized()
{
    auto box = getLocalBounds().reduced (10.0f);
    alphaVertical.setBounds (box.removeFromLeft(box.getWidth() / 3.0f));
    alphaVerticalSym.setBounds (box.removeFromLeft(box.getWidth() * 0.5f));
    alphaVerticalSymGradient.setBounds (box);

}
