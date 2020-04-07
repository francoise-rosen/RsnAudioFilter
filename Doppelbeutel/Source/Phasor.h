/*
  ==============================================================================

    Phasor.h
    Created: 5 Mar 2020 10:44:49am
    Author:  SFD

    Generates Phase ramp
  ==============================================================================
*/

#pragma once

class Signal
{
public:
    Signal()
    :currentAngle(0.0),
    deltaAngle(0.0)
    {};
    
    virtual forcedinline double getNextSample() noexcept
    {
        return currentAngle += deltaAngle;
    };
    
    // each derived class must have frequency
    virtual void updateFrequency(double) = 0;
    
    virtual ~Signal() {};
    
    // instantaneous frequency
private:
    double currentAngle;
    double deltaAngle;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Signal)
    
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
    
    void reset() {
        currentAngle = 0.0;
        }
    

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
    forcedinline double& getCurrentAngle() {return currentAngle;} // phase modulator will change this
    forcedinline void modulate(double mod)
    {
        currentAngle *= mod;
    }
    double getAngleDelta() const {return angleDelta;}
    
    virtual ~Phasor() {};
    
private:
    double localSampleRate;
    double localFrequency;
    double currentAngle;
    double angleDelta;
    bool isReseted;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Phasor)
    
};
