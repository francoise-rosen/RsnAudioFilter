#pragma once

#include <JuceHeader.h>
#include "RosenSliderLookAndFeel.h"
#include "AlphaOneLookAndFeel.h"
#include "TwoThreeValue.h"
#include "Bar.h"
#include "Rotary.h"
#include "LinearHorizontal.h"
#include "LinearVertical.h"

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
    LinearVertical linearVerticalSliders;
    LinearHorizontal linearHorizontalSliders;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
