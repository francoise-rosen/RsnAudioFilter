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
    addAndMakeVisible(&lfoRateSlider);
    addAndMakeVisible(&label_A);
    label_A.setText ("A", juce::NotificationType::dontSendNotification);
    label_A.setJustificationType (juce::Justification::centred);
    label_A.setColour (juce::Label::outlineColourId, juce::Colours::darkblue);
    
    setLookAndFeel (&waveshaperLookAndFeel);
}

LFOSection::~LFOSection()
{
    setLookAndFeel (nullptr);
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
  
}

void LFOSection::resized()
{
    auto box = getLocalBounds();
    lfoRateSlider.setBounds(box.removeFromLeft (box.getWidth() * 0.25f).removeFromBottom (getHeight() * 0.75f).reduced (5.0f));
    label_A.setBounds(getLocalBounds().withLeft(getWidth() * 0.75f).withBottom(getHeight() * 0.25f));

}
