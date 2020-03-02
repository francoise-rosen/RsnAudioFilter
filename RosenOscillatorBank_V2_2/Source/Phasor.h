/*
  ==============================================================================

    Phasor.h
    Created: 7 Feb 2020 11:54:08am
    Author:  SFD
 
    Phasor object for a fundamental freq osc. Rate objects for partials
    Rate objects copies Phasor
 
    Rate:
    1. Sync to master Phasor (reset(), frequency change)
    2. Export Phasor -> Rate, study the code

  ==============================================================================
*/

#pragma once

// scale function?
// conditions for rate to exist?

// this abstract class is to be used as disguise
class Signal
{
public:
    Signal()
    :currentAngle(0.0),
    deltaAngle(0.0)
    {};
    
    virtual forcedinline double getNextSample()
    {
        return currentAngle += deltaAngle;
    };
    
    virtual void updateFrequency(double) = 0;
    
    virtual ~Signal() {};
    
    // instantaneous frequency
private:
    double currentAngle;
    double deltaAngle;
    
};

// simple non-bandlimited phasor ramp

class Phasor : public Signal{
public:
    Phasor(double frequency, double sampleRate)
    :localSampleRate(sampleRate),
    localFrequency(frequency),
    currentAngle(0.0)
    {
        updateFrequency(localFrequency);
    }
    
    Phasor(Phasor& phaseToCopy)
    {
        currentAngle = phaseToCopy.currentAngle;
        angleDelta = phaseToCopy.angleDelta;
        localFrequency = phaseToCopy.localFrequency;
        localSampleRate = phaseToCopy.localSampleRate;
        std::cout << "copying Phasor..\n";
    }
    
    
    void updateFrequency(double frequency) noexcept override
    {
        
        if (localSampleRate > 0)
        {
        auto cyclesPerSample = frequency / localSampleRate;
        angleDelta = cyclesPerSample;
        }
        
    }
    
    virtual forcedinline double getNextSample() noexcept override
    {
        auto currentPhase = currentAngle;
        currentAngle += angleDelta;
        if(currentAngle == 1) currentAngle -= currentAngle;
        return currentPhase;
    }
    
    void reset() {currentAngle = 0.0;}
    

    // clock multiplier / devider
    // check this out:
    // https://cycling74.com/tutorials/my-favorite-object-rate~
    // export Phasor -> Rate from Gen, study the code!
    
//    forcedinline double rateSample(float rate) noexcept
//    {
//    // I must implement a wrap function here that does not affect getNextSample() stream
//    // or currentAngle and currentPhase
//    // make it with modulo?
//
//    }
    double getCurrentAngle() const {return currentAngle;}
    double getAngleDelta() const {return angleDelta;}
    
    virtual ~Phasor() {};
    
private:
    double localSampleRate;
    double localFrequency;
    double currentAngle;
    double angleDelta;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Phasor)
    
};

// complete after 8 Osc model done
// as Phasor the output must not exceed [0, 1) range

// rate copies a phasor
// copy constructor and copy assignment must be implemented in Phasor class

// we need to be able access frequency, phase offset (initial) and reset() here

// inherit from Phasor? we need to use it as phasor ramp for oscillator perhaps
class Rate : public Signal
{
public:
    Rate(Phasor& input, double rate)// signal to scale
    :localRate(rate) // rate of scale, can be a signal too
    {
        // first try range (0.25, 3)
        jassert(localRate > 0.0); // or it can be negative too?
        ratePhasor.reset(new Phasor(input));// copy constructor called
    }
    
    // small ratios like 0.25 is a clock divider
    // large ratios is a clock multiplier
    virtual forcedinline double getNextSample() noexcept override
    {
        auto sample = ratePhasor->getNextSample();
        return sample;
    }
    
    virtual void updateFrequency(double newRate) noexcept override
    {
        // this must update currentAngle and angleDelta
    }
    virtual ~Rate() {};
    
private:
    double localRate;
    double currentAngle; // we need to keep track
    double localFrequeny;
    double masterFreq;
    bool reset();
    std::unique_ptr<Phasor> ratePhasor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Rate)
};


