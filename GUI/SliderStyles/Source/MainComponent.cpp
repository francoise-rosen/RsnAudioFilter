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
    addAndMakeVisible(&twoValVertical);
    addAndMakeVisible(&twoValHorizontal);
    addAndMakeVisible(&threeValVertical);
    addAndMakeVisible(&threeValHorizontal);
    setSize (600, 400);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{

    g.fillAll (juce::Colours::silver.darker());

}

void MainComponent::resized()
{
    auto area = getLocalBounds().removeFromLeft (getWidth() * 0.5f).removeFromTop (getHeight() * 0.5f).reduced (5.0f);
    rotaryVerticalDrag.setBounds(area.removeFromLeft(area.getWidth()/3));
    rotaryHorizontalDrag.setBounds(area.removeFromLeft(area.getWidth()/2));
    rotaryVerticalHorizontalDrag.setBounds(area);
    
    auto linearArea = getLocalBounds().removeFromRight (getWidth() * 0.5f).removeFromTop (getHeight() * 0.5f).reduced (5.0f);
    
    auto linearHArea = linearArea.removeFromLeft (linearArea.getWidth() * 0.5f);
    linearH.setBounds(linearHArea.removeFromTop (linearHArea.getHeight() * 0.5f));
    linearBar.setBounds(linearHArea);
    
    linearV.setBounds (linearArea.removeFromLeft (linearHArea.getWidth() * 0.5f));
    linearBarV.setBounds(linearArea);
    
    auto threeTwoBox = getLocalBounds().removeFromBottom(getHeight() * 0.5f);
    auto threeTwoHBox = threeTwoBox.removeFromLeft (getWidth() * 0.5f);
    
    twoValHorizontal.setBounds(threeTwoHBox.removeFromTop(threeTwoHBox.getHeight() * 0.5f));
    threeValHorizontal.setBounds(threeTwoHBox);
    
    twoValVertical.setBounds(threeTwoBox.removeFromLeft(threeTwoBox.getWidth() * 0.5f));
    threeValVertical.setBounds(threeTwoBox);
    
    
 
}
