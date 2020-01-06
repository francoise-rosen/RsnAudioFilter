/*
  ==============================================================================

    Oscillator.h
    Created: 5 Jan 2020 2:49:42pm
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

// OSCILLATOR CLASS - direct computation?

// idea detune amount - maximum deviation from the main frequency
// idea - Spread detuned copies (1-16) with encapsulated gain control


// LFO class

// levels of both oscillators (must be normalised)
// frequencies of both oscillators
// index of modulation

class LFO : public Signal
{
public:
    LFO::LFO();
    forcedinline float getNextSample() override;
    void setFrequency(float frequency, float sampleRate);
    
private:
    enum LFOType
    {
        sine,
        sine2sync,
        sah,
        saw,
        tri,
        rect // with pulse width control
    }
}


// OSCILLATOR PROCESSING CLASS



enum ModType{
    sum,
    RM,
    AM,
    FMs
}

// ProcessorVCO(Oscillator* carrier, Oscillator* modulator, ModType modType)
