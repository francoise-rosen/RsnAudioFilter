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
    
    // this should not work
    // clock multiplier / devider
    // check this out:
    // https://cycling74.com/tutorials/my-favorite-object-rate~
    
//    forcedinline double rateSample(float rate) noexcept
//    {
//    // I must implement a wrap function here that does not affect getNextSample() stream
//    // or currentAngle and currentPhase
//    // make it with modulo?
//
//    }
    
    
    
private:
    double localSampleRate;
    double localFrequency;
    double currentAngle;
    double angleDelta;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Phasor)
    
};

// complete after 8 Osc model done
// as Phasor the output must not exceed [0, 1) range
class Rate {
public:
    Rate(Phasor& input, double rate)
    :p(input), // signal to scale
    localRate(rate) // rate of scale, can be a signal too
    {
        // first try range (0.25, 3)
        jassert(localRate > 0.0);
    }
    
    // this suppose to work as clock divider for 0.25, 0.5, 1.0 rate values
    // this probably works as hard sync now since it turns to 0 at phasor's phase 0 for
    // uneven ratios
    forcedinline double getNextSample() noexcept
    {
        double sample = std::fmod(p.getNextSample(), localRate);
        if(sample >= localRate) sample -= sample;
        sample /= localRate;
        // just for testing
        jassert(sample <= 1.0);
        return sample;
    }
    
private:
    double localRate;
    double currentAngle; // we need to keep track
    Phasor& p;

};
