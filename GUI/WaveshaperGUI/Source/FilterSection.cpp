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

}

FilterSection::~FilterSection()
{
}

void FilterSection::paint (juce::Graphics& g)
{

    g.fillAll (localBackground);
    
    auto area = getLocalBounds().reduced (edge);
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
}

void FilterSection::resized()
{

    

}
