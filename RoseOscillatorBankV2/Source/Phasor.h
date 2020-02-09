/*
  ==============================================================================

    Phasor.h
    Created: 7 Feb 2020 11:54:08am
    Author:  SFD

  ==============================================================================
*/

#pragma once

// simple non-bandlimited phasor ramp

class Phasor{
public:
    Phasor(double frequency, double sampleRate)
    :localSampleRate(sampleRate),
    localFrequency(frequency),
    currentAngle(0.0)
    {
        updateFrequency(localFrequency);
    }
    
    void updateFrequency(double frequency) noexcept
    {
        
        if (localSampleRate > 0)
        {
        auto cyclesPerSample = frequency / localSampleRate;
        angleDelta = cyclesPerSample;
        }
        
    }
    
    forcedinline double getNextSample() noexcept
    {
        auto currentPhase = currentAngle;
        currentAngle += angleDelta;
        if(currentAngle == 1) currentAngle -= currentAngle;
        return currentPhase;
    }
    
    void reset() {currentAngle = 0.0;}
    
    
private:
    double localSampleRate;
    double localFrequency;
    double currentAngle;
    double angleDelta;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Phasor)
    
};
