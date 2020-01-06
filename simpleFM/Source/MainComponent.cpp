/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

// PHASOR

Phasor::Phasor()
{}

void Phasor::setFrequency(float frequency, float sampleRate)
{
    // running from 0.0 to 2pi
    auto cyclesPerSample = frequency / sampleRate;
    deltaPhase = MathConstants<float>::twoPi * cyclesPerSample;
    
}

// phaser output is here->
forcedinline float Phasor::getNextPhase() noexcept
{
    auto currentSample = currentPhase;
    updateCount();
    return currentSample;
    
}
// wrap the count [0, 2pi)

forcedinline void Phasor::updateCount() noexcept
{
    currentPhase += deltaPhase;
    if(currentPhase >= MathConstants<float>::twoPi)
        currentPhase -= MathConstants<float>::twoPi;
}


// OSCILLATOR

Oscillator::Oscillator()
{}

Oscillator::Oscillator(Phasor* p)
:phase(p)
{}

forcedinline float Oscillator::getNextSample() noexcept
{
    auto currentPhase = phase->getNextPhase();
    // phasewrap here in case of phase modulation;
    if(currentPhase >= MathConstants<float>::twoPi)
        currentPhase -= MathConstants<float>::twoPi;
    auto currentSample = std::sin(currentPhase);
    return currentSample;
}


//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);
    
    gainSlider.setRange(0.0, 1.0);
    gainSlider.setValue(0.0);
    gainSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    addAndMakeVisible(gainSlider);
    gainSlider.addListener(this);
 
    setAudioChannels (2, 2);

}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    auto* phasorCarrier = new Phasor();
    phasorCarrier->setFrequency(100, (float)sampleRate);
    auto* phasorModulator = new Phasor();
    phasorModulator->setFrequency(256, (float)sampleRate);
    auto* oscillatorModulator = new Oscillator(phasorModulator);
    auto* oscillatorCarrier = new Oscillator(phasorCarrier);
    oscillators.add(oscillatorCarrier);
    oscillators.add(oscillatorModulator);
    level = gainSlider.getValue();

}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    auto* leftOutput = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* rightOutput = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
    bufferToFill.clearActiveBufferRegion();
    
    for(int i = 0; i < bufferToFill.numSamples; ++i)
    {
        // RM
//        auto currentSample = oscillators.getUnchecked(0)->getNextSample() * oscillators.getUnchecked(1)->getNextSample();
        // AM
        auto currentSample = oscillators.getUnchecked(0)->getNextSample() * (oscillators.getUnchecked(1)->getNextSample()/2.0f + 0.5f);
        leftOutput[i] = currentSample * (float)level;
        rightOutput[i] = currentSample * (float)level;
    }
}

void MainComponent::releaseResources()
{

}

//=============================================================================
// Slider

void MainComponent::sliderValueChanged(Slider* slider)
{
    if(slider == &gainSlider)
    {
        level = gainSlider.getValue();
    }
}


//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
// Gain Slider
    auto border = 10;
    auto gainSliderArea = getLocalBounds();
    gainSliderArea = gainSliderArea.removeFromBottom(getHeight()/4).removeFromRight(getWidth()/8).reduced(border);
    gainSlider.setBounds(gainSliderArea);
    
// CPU Labels
// Frequency Slider
// Pan Dial
}
