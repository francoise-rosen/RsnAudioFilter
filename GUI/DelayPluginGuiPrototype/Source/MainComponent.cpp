#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    auto width = dimentions.first * scaler;
    auto height = dimentions.second * scaler;
    setSize (width, height);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{

    g.fillAll (juce::Colours::white.withAlpha(0.75f));
    g.setColour(juce::Colours::orange.withBrightness(0.77f));
    auto frame = getLocalBounds().reduced(5.0f);
    g.drawRoundedRectangle(frame.toFloat(), 5.0f, 3.0f);
}

void MainComponent::resized()
{
  
}
