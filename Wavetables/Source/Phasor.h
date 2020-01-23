/*
  ==============================================================================

    Phasor.h
    Created: 22 Jan 2020 9:40:34am
    Author:  SFD
 
    1. Make simple Phasor work (just 0..1, 0..2pi ramp) on Sinewave input
    2. Make rate work, create copy constructor / assignment
    3. Make modulation work. First on this, so it changes currentAngle
    4. Make modulation work through copying the phasor object

  ==============================================================================
*/

#pragma once

// base class Signal

//class Signal
//{
//public:
//    Signal();
//    
//    virtual ~Signal();
//public:
//    double deltaAngle;
//}

class Phasor
{
public:
    Phasor()
    :localFrequency(0.0),
    localSampleRate(0.0),
    currentAngle(0),
    angleDelta(0)
    {}
    
    Phasor(double frequency, double sampleRate)
    :localFrequency(frequency),
    localSampleRate(sampleRate),
    currentAngle(0.0)
    {
        jassert(sampleRate > 0);
    }
    
    void setFrequency(double frequency, double sampleRate)
    {
        jassert(frequency >= 0 && frequency < sampleRate / 2 && sampleRate > 0);
        localSampleRate = sampleRate;
        angleDelta = frequency / sampleRate;
    }
    
    // modulate
    
    // create a copy of a Phasor object and modulate it's currentAngle?
    
    // or
    
//    double modulate(double index, double harmonicity)
//    {
//        double modulatedSample = getNextSample() +
//
//        return modulatedSample;
//    }
    
    // but I need to call it every sample
    // or maybe I use here disguised Signal?
    
    /*
     
     Signal modulate(Signal* s)
     {
       Signal local;
       local.setCurrentAngle = this->getCurrentSample() + s->getCurrentSample;
     
     }
     
     or
     double modulate(Signal* car, Signal* mode)
     {
       Signal local;
       local.setCurrentAngle = this->getCurrentSample() + s->getCurrentSample;
    
      return local->getNextSample()
     }
     
     */
    
    // modulate the original object
    // replace Phasor* ph with Signal* modulator
    forcedinline void modulateThis(Phasor* ph)
    {
        currentAngle += ph->getNextSample();
    }
    
    
    // phasewrap

    // getNextSample
    forcedinline double getNextSample() noexcept
    {
        if( localSampleRate > 0)
           currentAngle += angleDelta;
        
        if(currentAngle == 1)
            currentAngle -= currentAngle;
    
        
        return currentAngle;
    }
    
    // get phase in radians (for direct sine)
    
    forcedinline double getNextRadian() noexcept
    {
        double phaseInRad = getNextSample() * MathConstants<float>::twoPi;
        return phaseInRad;
    }
    
    // copy constructor and assignment?
    
    
    
    // make a copy of phasor here
//    forcedinline double applyRate(double rate) noexcept
//    {
//        // requires copyconstructor
//        // for future use Signal instead of Phasor
//        Phasor copyPhase{this};
//    }
            
    virtual ~Phasor();
    
    // modulate(Signal* signal); // can be another phasor or oscillator
    
public:
    double localFrequency;
    double localSampleRate;
    double currentAngle; // 0 to 1
    double angleDelta; // increment per sample
};
