#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    addAndMakeVisible(&slider1Frame);
    setSize (600, 400);
}

MainComponent::~MainComponent()
{
    DBG("MAIN COMPONENT DISTRUCTED\n");
    //setLookAndFeel(nullptr);
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    juce::Colour outerColour = juce::Colours::darkgreen;
    juce::Colour innerColour = juce::Colours::darkred;

    g.setGradientFill({outerColour, 0, 0, innerColour, 0, getWidth()/2.0f, true});
}

void MainComponent::resized()
{
    slider1Frame.setBounds(getLocalBounds().removeFromLeft(300).removeFromTop(300));
}
