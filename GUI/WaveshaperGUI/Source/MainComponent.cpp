#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    filterSection = std::make_unique<FilterSection>(backgroundTheme);
    addAndMakeVisible(*filterSection);
    crossfadeSection = std::make_unique<CrossfadeSection>(backgroundTheme);
    addAndMakeVisible(*crossfadeSection);
    saturationSection = std::make_unique<SaturationSection>(backgroundTheme);
    addAndMakeVisible(*saturationSection);
    setSize (750, 400);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::lightgrey.brighter());
    
    g.setColour (juce::Colours::black);
    g.drawRect (filterSection->getBounds());
    g.drawRect (saturationSection->getBounds());
    g.drawRect (crossfadeSection->getBounds());
    g.drawRect (lfoSection.getBounds());
    g.drawRect (gainSection.getBounds());
    
}

void MainComponent::resized()
{
    auto windowArea = getLocalBounds().reduced(edge);
    filterSection->setBounds (windowArea.removeFromLeft (windowArea.getWidth()* 2.0f / 5.0f));
    auto midSection = windowArea.removeFromLeft (windowArea.getWidth() * 2.0f / 3.0f);
    lfoSection.setBounds (midSection.removeFromBottom (midSection.getHeight() / 3.0f));
    saturationSection->setBounds (midSection);
    gainSection.setBounds (windowArea.removeFromBottom (windowArea.getHeight() / 3.0f));
    crossfadeSection->setBounds (windowArea);
}
