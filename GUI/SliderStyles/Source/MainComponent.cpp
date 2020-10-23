#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    addAndMakeVisible (&linearVerticalSliders);
    addAndMakeVisible (&linearHorizontalSliders);
    setSize (1200, 800);
}

MainComponent::~MainComponent()
{

}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::silver.brighter());

}

void MainComponent::resized()
{
    auto fullArea = getLocalBounds().reduced (5.0f);
    auto upperRow = fullArea.removeFromTop (fullArea.getHeight() * 0.5f);
    /** LinearHorizontal - LinearVertical  - BarHorizontal - BarVertical 3-2-2-1 */
    linearHorizontalSliders.setBounds (upperRow.withRight(upperRow.getWidth() * 3.0f / 8.0f));
    linearVerticalSliders.setBounds (upperRow.withLeft (upperRow.getWidth() * 3.0f / 8.0f).withRight (fullArea.getWidth() * 5.0f / 8.0f));
 
}
