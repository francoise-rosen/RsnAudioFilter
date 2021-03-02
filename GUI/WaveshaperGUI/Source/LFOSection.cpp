/*
  ==============================================================================

    LFOSection.cpp
    Created: 20 Sep 2020 12:27:44pm
    Author:  syfo_dias

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LFOSection.h"

//==============================================================================
LFOSection::LFOSection()
{
    addAndMakeVisible (&lfoRateSlider);
    addAndMakeVisible (&label_A);
    addAndMakeVisible (&toggle_a);
    addAndMakeVisible (&box_a);
    addAndMakeVisible (&destinationSlider_a);
    label_A.setText ("LFO", juce::NotificationType::dontSendNotification);
    label_A.setJustificationType (juce::Justification::centred);
    label_A.setColour (juce::Label::outlineColourId, juce::Colours::darkblue);
    label_A.setColour (juce::Label::backgroundColourId, juce::Colours::darkcyan.darker());
    
    setLookAndFeel (&waveshaperLookAndFeel);
}

LFOSection::~LFOSection()
{
    setLookAndFeel (nullptr);
}

void LFOSection::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::silver);   // clear the background

    g.setColour (juce::Colours::black);
    g.setFont (14.0f);
    g.drawText ("LFOSection", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
  
}

void LFOSection::resized()
{
    auto box = getLocalBounds();
//    lfoRateSlider.setBounds(box.removeFromLeft (box.getWidth() * 0.25f).removeFromBottom (getHeight() * 0.75f).reduced (5.0f));
//    label_A.setBounds(getLocalBounds().withLeft(getWidth() * 0.75f).withBottom(getHeight() * 0.25f));
//
    rateArea = std::make_unique<juce::Rectangle<int>> (box.removeFromLeft (box.getWidth() * 0.3f));
    phaseArea = std::make_unique<juce::Rectangle<int>> (box.removeFromLeft (box.getWidth() * 0.4f));
    sendArea = std::make_unique<juce::Rectangle<int>> (box);
    lfoRateSlider.setBounds (rateArea->reduced (5.0f));
    
    for (int j = 0; j < controls.size(); ++j)
    {
        controls[j].area_a = std::make_unique<juce::Rectangle<int>> (sendArea->withBottom (sendArea->getY() + sendArea->getHeight() / (4 - j)));
    }
    
}

/** Copy constructor. */
//LFOSection::SendControl::SendControl (const SendControl& send)
//{
//    /** does make unique deletes the object currently managed by unique_ptr? */
//    toggle = std::make_unique<juce::ToggleButton> ();
//    toggle = std::make_unique<juce::ToggleButton> (*(send.toggle));
//    /** do I need to dereference? */
//    amount = std::make_unique<juce::Slider> (send.amount);
//    destination = std::make_unique<juce::ComboBox> (send.destination);
//    area_a = std::make_unique<juce::Rectangle<int>> (send.area_a);
//}

void LFOSection::makeControls()
{
    controls.clear();
    controls.resize (numSends);
    for (int i = 0; i < controls.size(); ++i)
    {
//        controls[i].amount->setSliderStyle (juce::Slider::SliderStyle::Rotary);
        controls[i].toggle = std::make_unique<juce::ToggleButton> ("Send " + juce::String (i));
        controls[i].amount = std::make_unique<juce::Slider> (juce::Slider::SliderStyle::Rotary, juce::Slider::TextEntryBoxPosition::TextBoxBelow);
        controls[i].destination = std::make_unique<juce::ComboBox> ("Choice " + juce::String (i));
        controls[i].destination->addItemList({"iF FReQ", "iF Q", "iF Bst", "oF Freq", "oF Q", "oF Bst", "Satur", "Crossfd", "mx"}, 0);
    }
}

