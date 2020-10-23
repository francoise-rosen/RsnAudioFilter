/*
  ==============================================================================

    LinearHorizontal.cpp
    Created: 21 Oct 2020 4:56:36pm
    Author:  syfo_dias

  ==============================================================================
*/
#include <JuceHeader.h>
#include "LinearHorizontal.h"


LinearHorizontal::LinearHorizontal()
{
    initialiseSliders();
    setLookAndFeel (&alphaOneLF);
    sliders[sym]->setLookAndFeel (&alphaOneSymLF);
    sliders[symGradient]->setLookAndFeel(&alphaOneSymGradLF);
}

LinearHorizontal::~LinearHorizontal()
{
    setLookAndFeel (nullptr);
    sliders[sym]->setLookAndFeel(nullptr);
    sliders[symGradient]->setLookAndFeel (nullptr);
}

void LinearHorizontal::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::mediumpurple);
}

void LinearHorizontal::resized()
{
    auto box = getLocalBounds().removeFromRight (getWidth() * 0.8f).reduced (10.0f);
    auto sliderHeight = box.getHeight() / static_cast<float> (sliders.size());
    
    for (const auto& slider : sliders)
    {
        if (slider != nullptr)
        {
            slider->setBounds (box.removeFromTop (sliderHeight));
        }
    }
}

void LinearHorizontal::initialiseSliders()
{
    sliders.clear();
    sliders.resize (numSliders);
    for (int i = 0; i < numSliders; ++i)
    {
        sliders[i] = std::make_unique<juce::Slider> (juce::Slider::LinearHorizontal, juce::Slider::TextEntryBoxPosition::TextBoxBelow);
        addAndMakeVisible(*sliders[i]);

    }
    
    jassert (sliders.size() == numSliders);
}
