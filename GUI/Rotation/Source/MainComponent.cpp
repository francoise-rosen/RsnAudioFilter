#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (600, 400);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::silver.brighter());
    g.setColour (juce::Colours::red);
    
    juce::Rectangle<float> centreArea {5.0f, 5.0f};
    juce::Point<float> centrePoint {getWidth() * 0.5f, getHeight() * 0.5f};
    g.fillEllipse (centreArea.withCentre (centrePoint));
    
    float diameter = juce::jmin (getWidth() * 0.25f, getHeight() * 0.25f);
    auto thumbArea {centreArea.withSize  (diameter, diameter).withCentre (centrePoint)};
    g.setColour (juce::Colours::yellow);
    g.drawEllipse (thumbArea, 5.0f);
    
    juce::Path tri;
    tri.startNewSubPath(thumbArea.getX() + diameter * 0.5f, thumbArea.getY());
    tri.lineTo(thumbArea.getX() + diameter, thumbArea.getY() + diameter * 0.79f);
    tri.lineTo(thumbArea.getX(), thumbArea.getY() + diameter * 0.79f);
    tri.closeSubPath();
    g.setColour(juce::Colours::blue);
    g.strokePath(tri, juce::PathStrokeType (5.0f));
    
    g.setColour (juce::Colours::blueviolet);
    g.addTransform(juce::AffineTransform::rotation(juce::MathConstants<float>::halfPi * 2, centrePoint.getX(), centrePoint.getY()));
    g.strokePath(tri, juce::PathStrokeType (5.0f));
}

void MainComponent::resized()
{

}
