#pragma once

#include <JuceHeader.h>
#include "FilterSection.h"
#include "SaturationSection.h"
#include "LFOSection.h"
#include "GainSection.h"
#include "CrossfadeSection.h"

//==============================================================================
/** Colour palette:
 Surface: Black (filter triangles, crossfade area background)
          Dirty white / silver - the rest of background
 Rotary Slider Big (Saturation Slider)
    Rotary Slider Small (all other sliders)
    Linear Slider
    On Off button
    Lines, arrows. Gold for area edges, rest is black or white depending on background
    Combobox (2 colour scheme - black white, experiment with merges)
    Labels - 2 colour scheme - black white, depending on background
 */

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
    std::unique_ptr<FilterSection> filterSection;
    LFOSection lfoSection;
    std::unique_ptr<SaturationSection> saturationSection;
    std::unique_ptr<CrossfadeSection> crossfadeSection;
    GainSection gainSection;
    
    const float edge {5.0f};
    
    /** Colours */
    juce::Colour backgroundTheme {juce::Colours::silver.brighter()};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
