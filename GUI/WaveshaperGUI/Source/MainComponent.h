#pragma once

#include <JuceHeader.h>
#include "FilterSection.h"
#include "SaturationSection.h"
#include "LFOSection.h"
#include "GainSection.h"
#include "CrossfadeSection.h"

//==============================================================================

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
    FilterSection filterSection;
    LFOSection lfoSection;
    SaturationSection saturationSection;
    CrossfadeSection crossfadeSection;
    GainSection gainSection;
    
    const float edge {5.0f};


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
