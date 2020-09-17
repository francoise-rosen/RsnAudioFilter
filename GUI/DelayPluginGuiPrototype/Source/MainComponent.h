#pragma once

#include <JuceHeader.h>
#include "DelayFrame.h"
#include "FilterFrame.h"
#include "GainFrame.h"

//==============================================================================
/*
 Parameters:
 KNOBS:
 1. Left Delay
 2. Right Delay
 3. Feedback
 4. Filter Frequency
 5. Filter Type
 6. Amount (dry wet)
 7. Gain
 8. Q
 
 Combo:
 1. Type of delay: Stereo, Ping / Pong, LCR (dry wet affects only delayed signal)
 
 Buttons:
 1. Filter On / Off
*/

class MainComponent  : public juce::Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
    std::pair<float, float> dimentions {3.6f, 5.0f};
    const float scaler {130.0f};
    const float edge {5.0f};
    
    DelayFrame delaySection;
    FilterFrame filterSection;
    GainFrame gainSection;
    
    struct ColourPalette
    {
        ColourPalette()
        {}
        
        ColourPalette(const juce::Colour a, const juce::Colour b, const juce::Colour c, const juce::Colour d, const juce::Colour e)
        :backgroundColour{a}, bodyColour{b}, rimColour{c}, shadowColour{d}, grid{e}
        {}
        
        juce::Colour backgroundColour;
        juce::Colour bodyColour;
        juce::Colour rimColour;
        juce::Colour shadowColour;
        juce::Colour grid;
    };
    
    ColourPalette colourPalette;
    float fontHeight {17.0f};
    const juce::Font font {"Monaco", "Bold", fontHeight};
    
    juce::TextButton filterButton;
    ButtonLookAndFeel buttonLookAndFeel;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
