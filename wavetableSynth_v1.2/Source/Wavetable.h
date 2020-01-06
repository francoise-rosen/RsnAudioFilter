/*
  ==============================================================================

    WavetableOscillator_V1.h
    Created: 31 Oct 2019 11:07:44am
    Author:  SFD

  ==============================================================================
*/

#pragma once

// wrap the wavetables here?
class WavetableOscillator_V1 {
public:
    WavetableOscillator_V1 (const AudioSampleBuffer& wavetableToUse)
    :wavetable(wavetableToUse),
    tableSize(wavetable.getNumSamples() - 1)
    // why -1? because wavetable[wavetable.tableSize) = wavetable[0]
    
    {
        jassert(wavetable.getNumChannels() == 1);
    }
    
    
    // phasor object here
    
    void setFrequency(float frequency, float sampleRate)
    {
        // here we need to update tableDelta
        
        // cycles per sample
        auto tableOverSampleRate = tableSize / sampleRate;
        tableDelta = frequency * tableOverSampleRate;
        
    }
    
    forcedinline float getNextSample() noexcept
    {
        auto index0 = (unsigned int)currentIndex; // truncates to lower int
        auto index1 = index0 + 1;
        
        // fraction is the difference between the currentIndex (float) and the lower index, which is index0
        auto fraction = currentIndex - (float)index0;
        
        // read samples
        auto* lookup = wavetable.getReadPointer(0);
        // store 2 temp values
        auto value0 = lookup[index0];
        auto value1 = lookup[index1];
        
        auto currentValue = value0 + fraction * (value1 - value0);
        
        if((currentIndex += tableDelta) > tableSize)
            currentIndex -= tableSize;
        
        return currentValue;
    }
    
    
private:
    float currentIndex = 0.0f;
    float tableDelta = 0.0f;
    const AudioSampleBuffer& wavetable;
    float tableSize = 0.0f;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WavetableOscillator_V1);
};

/*
 
 class Phasor {
 
    // inlets
    double frequency;
    double rate;
    // outlets;
 
    double phaseDelta; // this where phase modulation is injected
 
 };
 
 
 */


/* class Oscillator
 {
 
 public:
    enum waveType{
        SINE, SAW3, SAW5, SAW7, SAW9, TRI3, TRI5, TRI7, TRI9, RECT3, RECT5, RECT7, RECT9
 }
    OwnedArray<WavetableOscillator_V1> waveforms;
 
 };

 
 */
