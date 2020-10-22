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
    addAndMakeVisible (&linearBar2);
    
    linearHTwo.setRange (0.0f, 1.0f);
    linearHTwo.setValue (0.5f);
    linearVTwo.setRange (0.0f, 1.0f);
    linearVTwo.setRange (0.0f, 1.0f);
    linearBar2.setRange (-1.0f, 1.0f);
    linearBar2.setValue (0.0f);
    linearBar.setRange (-1.0f, 1.0f);
    linearBar.setValue (0.0f);
    
    /** Custom look and feel. */
    rosenLookAndFeel.setColour (juce::Slider::trackColourId, juce::Colours::blue.withAlpha(0.83f));
    alphaLookAndFeel.setColour (juce::Slider::trackColourId, juce::Colours::darkblue);
    alphaLookAndFeel.setColour (juce::Slider::thumbColourId, juce::Colours::red);
    alphaLookAndFeel.setColour (juce::Slider::backgroundColourId, juce::Colours::silver.darker());
    rosenLookAndFeel.setColour (juce::Slider::thumbColourId, juce::Colours::rebeccapurple);
    alphaOneSymmetricalSlider.setColour (juce::Slider::thumbColourId, juce::Colours::red);
    alphaOneSymmetricalSlider.setColour (juce::Slider::trackColourId, juce::Colours::blue.withAlpha(0.24f));
    alphaOneSymmetricalSlider.setColour (juce::Slider::backgroundColourId, juce::Colours::darkcyan);
    alphaTwo.setColour (juce::Slider::trackColourId, juce::Colours::purple);
    
    
    linearHTwo.setLookAndFeel (&alphaOneSymmetricalSlider);
    linearBar.setLookAndFeel (&alphaOneSymmetricalSlider);
    linearBarV.setLookAndFeel (&rosenLookAndFeel);
    linearH.setLookAndFeel (&alphaLookAndFeel);
    linearV.setLookAndFeel (&alphaOneSymmetricalSlider);
    linearVTwo.setLookAndFeel (&alphaOneSymmetricalSlider);
    twoValHorizontal.setLookAndFeel (&alphaLookAndFeel);
    rosenLookAndFeel.setThumbShape (RosenSliderLookAndFeel::ThumbShape::rhombus);
    twoValVertical.setLookAndFeel (&rosenLookAndFeel);
    threeValVertical.setLookAndFeel (&alphaLookAndFeel);
    threeValHorizontal.setLookAndFeel (&alphaLookAndFeel);
    linearBar2.setLookAndFeel (&alphaTwo);
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
    linearBar2.setLookAndFeel (nullptr);
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
    linearBar.setBounds (linearHArea.removeFromTop (linearHArea.getHeight() * 0.5f));
    linearBar2.setBounds (linearHArea);
    
    linearV.setBounds (linearArea.removeFromLeft (linearHArea.getWidth() * 0.5f));
    linearBarV.setBounds (linearArea);
    
    auto threeTwoBox = getLocalBounds().removeFromBottom(getHeight() * 0.5f);
    auto threeTwoHBox = threeTwoBox.removeFromLeft (getWidth() * 0.5f);
    
    twoValHorizontal.setBounds (threeTwoHBox.removeFromTop (threeTwoHBox.getHeight() * 0.5f));
    threeValHorizontal.setBounds (threeTwoHBox);
    
    twoValVertical.setBounds (threeTwoBox.removeFromLeft (threeTwoBox.getWidth() * 0.5f));
    threeValVertical.setBounds (threeTwoBox);
    
    
 
}
