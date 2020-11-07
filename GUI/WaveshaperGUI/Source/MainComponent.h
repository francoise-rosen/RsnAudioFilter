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
    Rotary Slider medium
    Rotary Slider Small (all other sliders)
    Linear Slider (LFO phase controls
    On Off button
    Lines, arrows. Gold for area edges, rest is black or white depending on background
    Combobox (2 colour scheme - black white, experiment with merges)
    Labels - 2 colour scheme - black white, depending on background
 */

/** LFO Parameters:
 - Frequency (manual or sync (what's the range for sync)
 - Waveform (Sine, Saw, Tri, Rect)
 - SAH (how ?)
 - Phase. LFO can output n waveforms at the same time (let's take 4). 4 slider for phase
  control.
 - N targets (symmetry, crossface, amount, filter cutoff, filter q, filter boost)
 - how do I set amounts of modulation (in LFO, in the receiver?)
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
    std::unique_ptr<GainSection> gainSection;
    
    int width { 750 };
    int height { 400 };
    const float edge {5.0f};
    
    /** Colours */
    juce::Colour backgroundTheme {juce::Colours::silver.brighter()};
    float fontHeight;
    std::unique_ptr<juce::Font> labelFont;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
