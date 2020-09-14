#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
:colourPalette(
               juce::Colours::white.withAlpha(0.45f).withBrightness(0.92f), /* background */
               juce::Colours::white.withAlpha(0.92f).withHue(0.72f), /* object colours */
               juce::Colours::black.withAlpha(0.77f), /* rims and text colour */
               juce::Colours::black, /* shadows and edges */
               juce::Colours::orange.withBrightness(0.77f)
               )

{
    
    auto width = dimentions.first * scaler;
    auto height = dimentions.second * scaler;
    addAndMakeVisible(&delaySection);
    filterSection.setBackgroundColour(colourPalette.backgroundColour);
    delaySection.setBackgroundColour(colourPalette.backgroundColour);
    filterSection.setFont(font);
    addAndMakeVisible(&filterSection);
    addAndMakeVisible(&gainSection);
    
    setSize (width, height);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{

    g.fillAll (juce::Colours::white.withAlpha(0.75f));
    g.setColour(colourPalette.grid);
    auto frame = getLocalBounds().reduced(5.0f);
    g.drawRoundedRectangle(frame.toFloat(), 5.0f, 3.0f);
  
}

void MainComponent::resized()
{
    auto area = getLocalBounds().reduced(5.0f);
    delaySection.setBounds(area.removeFromLeft(area.getWidth() * 0.567f));
    filterSection.setBounds(area.removeFromTop(area.getHeight() * 0.5f));
    gainSection.setBounds(area);
}
