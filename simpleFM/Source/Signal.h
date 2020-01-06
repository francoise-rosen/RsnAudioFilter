/*
  ==============================================================================

   Basic signal

  ==============================================================================
*/

#pragma once

class Signal{
public:
    Signal();
    virtual void setFrequency(float frequency, float sampleRate)
    virtual forcedinline float getNextSample()=0 noexcept;
    virtual ~Signal();
private:
    float currentPhase = 0.0;
    float deltaPhase = 0.0;
    //float currentSampleRate = 0.0;
    
};

class Phasor : public Signal
{
public:
    Phasor(); // do I need a default constructor here?
    void setFrequency(float frequency, float sampleRate);
    forcedinline float getNextSample() override noexcept;
    forcedinline void updateCount() noexcept; // if frequency changed;
    {
        // goes from 0.0 to 1.0
        
    }
private:
    float currentPhase = 0.0;
    float deltaPhase = 0.0;
    
};
