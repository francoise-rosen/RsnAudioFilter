#pragma once

#include <JuceHeader.h>

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
    std::pair<float, float> dimentions {4.0f, 5.0f};
    const float scaler {100.0f};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
