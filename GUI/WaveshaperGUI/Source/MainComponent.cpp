#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (750, 400);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::lightgrey.brighter());
    
    g.setColour(juce::Colours::black);
    
    g.drawRect(filterSection.getBounds());
    g.drawRect(saturationSection.getBounds());
    g.drawRect(crossfadeSection.getBounds());
    g.drawRect(lfoSection.getBounds());
    g.drawRect(gainSection.getBounds());
    
    
}

void MainComponent::resized()
{
    auto windowArea = getLocalBounds().reduced(edge);
    filterSection.setBounds(windowArea.removeFromLeft(windowArea.getWidth()* 2.0f / 5.0f));
    saturationSection.setBounds(windowArea.removeFromLeft(windowArea.getWidth() * 2.0f / 3.0f));
    crossfadeSection.setBounds(windowArea.removeFromTop(windowArea.getHeight() / 3.0f));
    lfoSection.setBounds(windowArea.removeFromTop(windowArea.getHeight() * 0.5f));
    gainSection.setBounds(windowArea);
}
