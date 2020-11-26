/*
  ==============================================================================

    FilterSection.cpp
    Created: 20 Sep 2020 12:27:10pm
    Author:  syfo_dias

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterSection.h"

//==============================================================================
FilterSection::FilterSection(juce::Colour parentBackground)
{
    localBackground = parentBackground;
    setLookAndFeel (&waveshaperLookAndFeel);
    intialiseFilterControls (inputFilter);
    intialiseFilterControls (outputFilter);
    inputFilter.algorithm.addItemList({"LPF", "HPF", "BPF"}, 100);
    inputFilter.algorithm.setSelectedItemIndex(0);
    outputFilter.algorithm.addItemList({"LPF", "HPF", "BPF"}, 100);

}

FilterSection::~FilterSection()
{
    setLookAndFeel (nullptr);
    //inputFilter.boost.setLookAndFeel (nullptr);
    outputFilter.boost.setLookAndFeel (nullptr);
}

void FilterSection::paint (juce::Graphics& g)
{

    g.fillAll (localBackground);
    
    auto area = getLocalBounds().reduced (edge);
    g.setColour (juce::Colours::red);
    g.drawRect(area);
    juce::Path diagonal;
    diagonal.startNewSubPath(area.getX(), area.getY());
    diagonal.lineTo(area.getX() + area.getWidth(), area.getY() + area.getHeight());
    g.strokePath(diagonal, juce::PathStrokeType {1.0f});
    
    
    g.setColour(juce::Colours::black);
    
    /** Output filter - left bottom secion triangle. */
    juce::Path leftTri;
    leftTri.startNewSubPath (area.getX(), area.getY() + edge);
    leftTri.lineTo (area.getX(), area.getBottom());
    leftTri.lineTo (area.getRight() - edge, area.getBottom());
    leftTri.closeSubPath();
    leftTri = leftTri.createPathWithRoundedCorners (edge * 2.0f);
    g.fillPath(leftTri);
    
    /** Input filter - right upper secion triangle. */
    juce::Path rightTri;
    rightTri.startNewSubPath (area.getRight(), area.getY() + edge);
    rightTri.lineTo (area.getRight(), area.getBottom());
    rightTri.lineTo (area.getX() + edge, area.getY() + edge);
    rightTri.closeSubPath();
    rightTri = rightTri.createPathWithRoundedCorners (edge * 3.0f);
    g.strokePath(rightTri, juce::PathStrokeType (2.0f));
    
    g.setColour (juce::Colours::orange);
    if (iFilterFreqKnobRect != nullptr)
    {
        g.drawRect(*iFilterFreqKnobRect);
    }
    
    if (iFilterQKnobRect != nullptr)
    {
        g.drawRect (*iFilterQKnobRect);
    }
    
    if (iFilterBoostKnobRect != nullptr)
    {
        g.drawRect (*iFilterBoostKnobRect);
    }
    
    if (oFilterQKnobRect != nullptr)
    {
        g.drawRect(*oFilterQKnobRect);
    }
    
    if (oFilterFreqKnobRect != nullptr)
    {
        g.drawRect(*oFilterFreqKnobRect);
    }
    
    if (oFilterBoostKnobRect != nullptr)
    {
        g.drawRect(*oFilterBoostKnobRect);
    }
    if (iFilterButtonRect != nullptr)
    {
        g.drawRect(*iFilterButtonRect);
    }
    
    if (oFilterButtonRect != nullptr)
    {
        g.drawRect(*oFilterButtonRect);
    }
    
    
}
/** Work on proper geometry (angles, edges and stuff.
 This concepts has some vague calculatons and rounding errors!.
 */
void FilterSection::resized()
{
    auto area = getLocalBounds().reduced (edge) ;
    
    //=====================================================================
    /** Input filter. */
    auto freqSliderSide = juce::jmin (getHeight() * 0.4f, getWidth() * 0.5f);
    auto midSliderSide = freqSliderSide * 0.67f;
    auto smallSliderSide = midSliderSide * 0.67f;
    auto buttonSide = smallSliderSide * 0.5f;
    
    iFilterFreqKnobRect = std::make_unique<juce::Rectangle<int>> (area.withTop(getHeight() - freqSliderSide).withRight (freqSliderSide).reduced (edge));
    iFilterQKnobRect = std::make_unique<juce::Rectangle<int>> (area.withBottom (getHeight() - freqSliderSide).withTop (getHeight() - freqSliderSide - midSliderSide).withRight(area.getX() + midSliderSide).reduced (edge));
    iFilterBoostKnobRect = std::make_unique<juce::Rectangle<int>> (area.withBottom (iFilterQKnobRect->getY()).withTop (iFilterQKnobRect->getY() - smallSliderSide).withRight (smallSliderSide).reduced (edge));
    iFilterButtonRect = std::make_unique<juce::Rectangle<int>>(area.withBottom (iFilterBoostKnobRect->getY()).withRight (area.getX() + buttonSide).withTop (iFilterBoostKnobRect->getY() - buttonSide).reduced (edge));
    
    inputFilter.frequency.setBounds (area.withTop(getHeight() - freqSliderSide).withRight (freqSliderSide).reduced (edge));
    inputFilter.algorithm.setBounds (area.withLeft (freqSliderSide).withTop (getHeight() * 0.9f).withRight (getWidth() * 0.9f).reduced (edge));
    inputFilter.q.setBounds (*iFilterQKnobRect);
    inputFilter.boost.setBounds (*iFilterBoostKnobRect);
    inputFilter.toggleButton.setBounds (*iFilterButtonRect);
    
    //=====================================================================
    /** Output filter. */
    oFilterQKnobRect = std::make_unique<juce::Rectangle<int>> ((*iFilterQKnobRect).withCentre({area.getCentreX(), static_cast<int>(area.getY() + edge + edge + iFilterQKnobRect->getHeight() * 0.5f)}));
    oFilterFreqKnobRect = std::make_unique<juce::Rectangle<int>> ((*iFilterFreqKnobRect).withCentre({area.getRight() - (int)(freqSliderSide * 0.5f - edge * 0.5f), (int)(area.getY() + midSliderSide * 0.75f + freqSliderSide * 0.5f)}));
    oFilterBoostKnobRect = std::make_unique<juce::Rectangle<int>> ((*iFilterBoostKnobRect).withCentre ({area.getRight() - (int)(smallSliderSide * 0.5f - edge * 0.5f), (int)(area.getY() + midSliderSide * 0.75f + freqSliderSide + smallSliderSide * 0.5f) }));
    oFilterButtonRect = std::make_unique<juce::Rectangle<int>>((*iFilterButtonRect).withCentre({static_cast<int>(area.getRight() - (int)(buttonSide * 0.5f)), (int)(oFilterBoostKnobRect->getBottom() + buttonSide * 0.5f) }));
    
    outputFilter.frequency.setBounds (*oFilterFreqKnobRect);
    outputFilter.q.setBounds (*oFilterQKnobRect);
    outputFilter.boost.setBounds (*oFilterBoostKnobRect);
    outputFilter.toggleButton.setBounds (*oFilterButtonRect);


}

void FilterSection::intialiseFilterControls (FilterControls &filterControls)
{
    addAndMakeVisible (& (filterControls.frequency) );
    addAndMakeVisible (& (filterControls.q) );
    addAndMakeVisible (& (filterControls.boost) );
    addAndMakeVisible (& (filterControls.algorithm) );
    addAndMakeVisible (& (filterControls.amount));
    addAndMakeVisible (& (filterControls.toggleButton));
    
    filterControls.frequency.setRange (20.0, 20000.0);
    filterControls.frequency.setSkewFactorFromMidPoint (1000.0);
    filterControls.frequency.setValue (1000.0);
    filterControls.frequency.setNumDecimalPlacesToDisplay (1);
    if (&filterControls == &inputFilter)
    {
        filterControls.frequency.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::silver.brighter());
        filterControls.frequency.setColour(juce::Slider::thumbColourId, juce::Colours::silver.brighter());
        filterControls.frequency.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::silver.brighter());
        filterControls.q.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::silver.brighter());
        filterControls.q.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::silver.brighter());
        filterControls.q.setColour(juce::Slider::thumbColourId, juce::Colours::silver.brighter());
    }
    
    else if (&filterControls == &outputFilter)
    {
        filterControls.frequency.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::blue.withBrightness(0.2f));
        filterControls.frequency.setColour(juce::Slider::thumbColourId, juce::Colours::blue.withBrightness(0.2f));
        filterControls.frequency.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::blue.withBrightness(0.2f));
        filterControls.q.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::blue.withBrightness(0.2f));
        filterControls.q.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::blue.withBrightness(0.2f));
        filterControls.q.setColour(juce::Slider::thumbColourId, juce::Colours::blue.withBrightness(0.2f));
    }
    
    filterControls.boost.setLookAndFeel (&smallSliderLookAndFeel);
//    filterControls.boost.setColour(juce::Slider::backgroundColourId, juce::Colours::silver);
}

