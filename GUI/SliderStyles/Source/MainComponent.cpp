#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    addAndMakeVisible(&rotaryVerticalDrag);
    addAndMakeVisible(&rotaryHorizontalDrag);
    addAndMakeVisible(&rotaryVerticalHorizontalDrag);
    addAndMakeVisible(&linearH);
    addAndMakeVisible(&linearV);
    addAndMakeVisible(&linearBar);
    addAndMakeVisible(&linearBarV);
    setSize (600, 400);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void MainComponent::resized()
{
    auto area = getLocalBounds().removeFromLeft (getWidth() * 0.5f).removeFromTop (getHeight() * 0.5f).reduced (5.0f);
    rotaryVerticalDrag.setBounds(area.removeFromLeft(area.getWidth()/3));
    rotaryHorizontalDrag.setBounds(area.removeFromLeft(area.getWidth()/2));
    rotaryVerticalHorizontalDrag.setBounds(area);
    
    auto linearArea = getLocalBounds().removeFromRight (getWidth() * 0.5f).removeFromTop (getHeight() * 0.5f).reduced (5.0f);
    
    linearH.setBounds(linearArea.removeFromTop (linearArea.getHeight() * 0.5f).removeFromLeft (linearArea.getWidth() * 0.5f));
 
}
