#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    addAndMakeVisible (&rotaryVerticalDrag);
    addAndMakeVisible (&rotaryHorizontalDrag);
    addAndMakeVisible (&rotaryVerticalHorizontalDrag);
    addAndMakeVisible (&linearH);
    addAndMakeVisible (&linearV);
    addAndMakeVisible (&linearBar);
    addAndMakeVisible (&linearBarV);
    addAndMakeVisible (&twoValVertical);
    addAndMakeVisible (&twoValHorizontal);
    addAndMakeVisible (&threeValVertical);
    addAndMakeVisible (&threeValHorizontal);
    addAndMakeVisible (&linearHTwo);
    addAndMakeVisible (&linearVTwo);
    
    /** Custom look and feel. */
    rosenLookAndFeel.setColour (juce::Slider::trackColourId, juce::Colours::blue.withAlpha(0.83f));
    alphaLookAndFeel.setColour (juce::Slider::trackColourId, juce::Colours::darkblue);
    alphaLookAndFeel.setColour (juce::Slider::thumbColourId, juce::Colours::red);
    alphaLookAndFeel.setColour (juce::Slider::backgroundColourId, juce::Colours::silver.darker());
    rosenLookAndFeel.setColour (juce::Slider::thumbColourId, juce::Colours::rebeccapurple);
    alphaOneSymmetricalSlider.setColour (juce::Slider::thumbColourId, juce::Colours::red);
    alphaOneSymmetricalSlider.setColour (juce::Slider::trackColourId, juce::Colours::blue);
    alphaOneSymmetricalSlider.setColour (juce::Slider::backgroundColourId, juce::Colours::darkcyan);
    
    
    linearHTwo.setLookAndFeel (&alphaOneSymmetricalSlider);
    linearBar.setLookAndFeel (&rosenLookAndFeel);
    linearBarV.setLookAndFeel (&rosenLookAndFeel);
    linearH.setLookAndFeel (&alphaLookAndFeel);
    linearV.setLookAndFeel (&alphaLookAndFeel);
    linearVTwo.setLookAndFeel (&alphaOneSymmetricalSlider);
    twoValHorizontal.setLookAndFeel (&alphaLookAndFeel);
    rosenLookAndFeel.setThumbShape (RosenSliderLookAndFeel::ThumbShape::rhombus);
    twoValVertical.setLookAndFeel (&rosenLookAndFeel);
    threeValVertical.setLookAndFeel (&alphaLookAndFeel);
    threeValHorizontal.setLookAndFeel (&alphaLookAndFeel);
    setSize (600, 400);
}

MainComponent::~MainComponent()
{
    linearBar.setLookAndFeel (nullptr);
    linearBarV.setLookAndFeel (nullptr);
    linearH.setLookAndFeel (nullptr);
    linearHTwo.setLookAndFeel (nullptr);
    linearV.setLookAndFeel (nullptr);
    twoValVertical.setLookAndFeel (nullptr);
    twoValHorizontal.setLookAndFeel (nullptr);
    threeValVertical.setLookAndFeel (nullptr);
    threeValHorizontal.setLookAndFeel (nullptr);
    linearVTwo.setLookAndFeel (nullptr);
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{

    g.fillAll (juce::Colours::silver.brighter());

}

void MainComponent::resized()
{
    auto area = getLocalBounds().removeFromLeft (getWidth() * 0.5f).removeFromTop (getHeight() * 0.5f).reduced (5.0f);
    rotaryVerticalDrag.setBounds (area.removeFromLeft (area.getWidth()/3));
    rotaryHorizontalDrag.setBounds (area.removeFromLeft (area.getWidth()/2));
    rotaryVerticalHorizontalDrag.setBounds (area);
    
    auto linearArea = getLocalBounds().removeFromRight (getWidth() * 0.5f).removeFromTop (getHeight() * 0.5f).reduced (5.0f);
    
    auto linearHArea = linearArea.removeFromLeft (linearArea.getWidth() * 0.5f);
    linearH.setBounds (linearHArea.removeFromTop (linearHArea.getHeight() / 3.0f));
    linearHTwo.setBounds (linearHArea.removeFromTop (linearHArea.getHeight() * 0.5f));
    linearBar.setBounds (linearHArea);
    
    linearV.setBounds (linearArea.removeFromLeft (linearHArea.getWidth() * 0.5f));
    linearBarV.setBounds (linearArea);
    
    auto threeTwoBox = getLocalBounds().removeFromBottom(getHeight() * 0.5f);
    auto threeTwoHBox = threeTwoBox.removeFromLeft (getWidth() * 0.5f);
    
    twoValHorizontal.setBounds (threeTwoHBox.removeFromTop (threeTwoHBox.getHeight() * 0.5f));
    threeValHorizontal.setBounds (threeTwoHBox);
    
    twoValVertical.setBounds (threeTwoBox.removeFromLeft (threeTwoBox.getWidth() * 0.5f));
    threeValVertical.setBounds (threeTwoBox);
    
    
 
}
