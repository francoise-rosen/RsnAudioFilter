/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
// MainComponent initialisation
// Size of the application window
// 
MainComponent::MainComponent()
{
    setSize (600, 400);
    addAndMakeVisible(dial1);
    dial1.setSliderStyle(Slider::SliderStyle::Rotary);
    dial1.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 25);
    
    addAndMakeVisible(dial2);
    dial2.setSliderStyle(Slider::SliderStyle::Rotary);
    dial2.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 25);
    
    getLookAndFeel().setColour(Slider::thumbColourId, Colours::red); // these are enums!
    getLookAndFeel().setColour(Slider::rotarySliderFillColourId, Colours::azure);
    
    objLookAndFeel.setColour(Slider::thumbColourId, Colours::beige);
    dial2.setLookAndFeel(&objLookAndFeel);
    dial1.setLookAndFeel(&my_look);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
}

void MainComponent::resized()
{
    const int border = 20;
    int dialWidth = getWidth() / 2 - border;
    int dialHeight = getHeight() - border - 100;
    dial1.setBounds(border, border, dialWidth, dialHeight);
    dial2.setBounds(getWidth()/2, border, dialWidth, dialHeight);
}
