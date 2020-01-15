/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

// customised LookAndFeel class that inherits from LookAndFeel_V4
class MyLookAndFeel : public LookAndFeel_V4
{
public:
    // default constructor
    MyLookAndFeel(){
        setColour(Slider::rotarySliderOutlineColourId, Colours::blanchedalmond);
        setColour(Slider::thumbColourId, Colours::chocolate);
        setColour(Slider::rotarySliderFillColourId, Colours::black);
    }
};


class MainComponent   : public Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
    Slider dial1;
    Slider dial2;
    
    LookAndFeel_V4 objLookAndFeel;
    MyLookAndFeel my_look;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
