#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
:colourPalette(
               juce::Colours::white.withAlpha(0.45f).withBrightness(0.92f), /* background */
               juce::Colours::white.withAlpha(0.92f).withHue(0.72f), /* object colours */
               juce::Colours::black.withAlpha(0.77f), /* rims and text colour */
               juce::Colours::black, /* shadows and edges */
               juce::Colours::orange.withBrightness(0.97f)
               )

{
    
    setLookAndFeel(&buttonLookAndFeel);
    
    auto width = dimentions.first * scaler;
    auto height = dimentions.second * scaler;
    addAndMakeVisible(&delaySection);
    filterSection.setBackgroundColour(colourPalette.backgroundColour);
    delaySection.setBackgroundColour(colourPalette.backgroundColour);
    filterSection.setFont(font);
    delaySection.setFont(font);
    delaySection.setTextColour(colourPalette.rimColour);
    addAndMakeVisible(&filterSection);
    addAndMakeVisible(&gainSection);
    
    filterButton.setColour(juce::TextButton::buttonColourId, juce::Colours::orange);
    filterButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::red);
    addAndMakeVisible(&filterButton);
    
    setSize (width, height);
}

MainComponent::~MainComponent()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{

    g.fillAll (juce::Colours::white.withAlpha(0.75f));
    g.setColour(colourPalette.grid);
    auto frame = getLocalBounds().reduced(edge);
    g.drawRoundedRectangle(frame.toFloat(), edge, 3.0f);
    
    /* delay to filter pointer */
    juce::Path toFilter;
    juce::Point<float> start {getWidth() * 0.567f * 0.9f, frame.getHeight() * 0.165f};
    juce::Point<float> end {frame.getWidth() * 0.567f + getWidth() * 0.07f, frame.getHeight() * 0.165f};
    toFilter.startNewSubPath(start);
    toFilter.lineTo(end);
    g.setColour(juce::Colours::black);
    g.strokePath(toFilter, juce::PathStrokeType(1.0f));
    
    /* custom arrow */
    juce::Path arrow;
    arrow.startNewSubPath(end.getX() + getWidth() * 0.02f, end.getY());
    arrow.lineTo(end.getX(), end.getY() - 5.0f);
    arrow.lineTo(end.getX(), end.getY() + 5.0f);
    g.fillPath(arrow);
    
    
    
}

void MainComponent::resized()
{
    auto area = getLocalBounds().reduced(edge);
    delaySection.setBounds(area.removeFromLeft(area.getWidth() * 0.567f));
    filterSection.setBounds(area.removeFromTop(area.getHeight() * 0.7f));
    gainSection.setBounds(area);
    
    juce::Point<float> buttonCentre {getWidth() * 0.567f, getHeight() * 0.082f};
    auto buttonLen = getHeight() * 0.05f;
    juce::Point<float> buttonXY {buttonCentre.getX() - buttonLen * 0.5f, buttonCentre.getY() - buttonLen * 0.5f};
    
    filterButton.setBounds(buttonXY.getX(), buttonXY.getY(), buttonLen, buttonLen);
}
