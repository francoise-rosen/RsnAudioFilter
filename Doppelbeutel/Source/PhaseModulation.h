/*
  ==============================================================================

    PhaseModulation.h
    Created: 5 Mar 2020 10:46:28am
    Author:  SFD
    
    takes 2 sine waves, accesses the phase of carrier and modulates it
    by output from modulator

  ==============================================================================
*/

#pragma once
#include "Sine.h"

class PhaseModulation
{
public:
    PhaseModulation()
    :carrier(nullptr), modulator(nullptr), modulationIndex(0),
    sampleRate(0), blockSize(0) {}
    
    PhaseModulation(SineOSC* c, SineOSC* m, double indMod, const double& sRate, const double& bSize = 0)
    :carrier(c),
    modulator(m),
    modulationIndex(indMod),
    sampleRate(sRate),
    blockSize(bSize)
    {}
    
    forcedinline void setIndex(double index)
    {
        modulationIndex = index;
    }
    
    AudioSampleBuffer& process(double& blockSize) noexcept
    {

        buffer.setSize(0, blockSize);
        buffer.clear();
        
        auto* outbuf = buffer.getWritePointer(0);
        
        
        // get write pointer!
        
        // Signal 1 currentAngle to be changed by Signal2->getNextSample() * modIndex;
        
        for (int i = 0; i < blockSize; ++i)
        {
            carrier->accessPhasor().modulate (modulator->getNextSample() * modulationIndex);
            auto sample = carrier->getNextSample();
            outbuf[i] = sample;
        }
        
        return buffer;
    }
    
private:
    SineOSC* carrier;
    SineOSC* modulator;
    double modulationIndex;
    double sampleRate;
    double blockSize;
    AudioSampleBuffer buffer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhaseModulation)
};


