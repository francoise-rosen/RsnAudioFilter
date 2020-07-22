/*
  ==============================================================================

    Biquad.h
    Created: 21 Jul 2020 1:15:49pm
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

namespace rosen
{
    enum class biquadAlgorithm {LPF, HPF, ButterLPF, ButterHPF, BPF, HiShelf, LoShelf, numOfAlgorithms};
    enum filterCoef {a0, a1, a2, b1, b2, c, d, numOfCoef};
    enum zState {x_z1, x_z2, y_z1, y_z2, numOfZ};
    
    //Biquad Class
    template <typename T>
    class Biquad
    {
    public:
        
        Biquad();
        
        Biquad(const T& freq, const T& q, double sampleRate, biquadAlgorithm algo = biquadAlgorithm::LPF);
        
        ~Biquad() {}
        
        // setters
        
        void setFrequency(const T& freq);
        void setQ(const T& q);
        void setFilterAlgorithm(const int&);
        void setSampleRate(const double& sampleRate);
        
        // getters
        T getFreq() const {return frequency;}
        T getQ() const {return qualityFactor;}
        biquadAlgorithm getAlgorithm() const {return algorithm;}
        
        void reset();
        void resetCoefficients();
        
        
    private:
        using Math = juce::MathConstants<double>;
        T frequency;
        T qualityFactor;
        double currentSampleRate;
        biquadAlgorithm algorithm;
        
        //static arrays
        T filterCoefficients[filterCoef::numOfCoef];
        T zArray[zState::numOfZ];
        
        // this must be called if and only if all the member variables are initialised
        // in particular currentSampleRate and Q can't be 0!
        void setCoefficients();
    };
    
    // CONSTRUCTORS
    template <typename T>
    Biquad<T>::Biquad()
    :frequency{0}, qualityFactor{1}, currentSampleRate{0}, algorithm{biquadAlgorithm::LPF}
    {}
    
    template <typename T>
    Biquad<T>::Biquad(const T& freq, const T& q, double sampleRate, biquadAlgorithm algo)
    :frequency{freq}, qualityFactor{q}, currentSampleRate{sampleRate}, algorithm{algo}
    {
        assert (currentSampleRate > 0);
        setCoefficients();
    }
    
    // SETTERS
    template <typename T>
    void Biquad<T>::setSampleRate(const double& sampleRate)
    {
        if (currentSampleRate == sampleRate) return;
        currentSampleRate = sampleRate;
        setCoefficients();
    }
    
    template <typename T>
    void Biquad<T>::setFrequency(const T& freq)
    {
        if (frequency == freq) return;
        frequency = freq;
        setCoefficients();
    }
    
    template <typename T>
    void Biquad<T>::setQ(const T& q)
    {
        if (qualityFactor == q) return;
        qualityFactor = q;
        setCoefficients();
    }
    
    template <typename T>
    void Biquad<T>::setFilterAlgorithm(const int& filterType)
    {
        if (algorithm == (biquadAlgorithm)filterType) return;
        
        if (filterType > (int)(biquadAlgorithm::numOfAlgorithms) - 1)
        {
            algorithm = biquadAlgorithm::LPF;
        }
        else algorithm = (biquadAlgorithm)filterType;
        setCoefficients();
    }
    
    template <typename T>
    void Biquad<T>::setCoefficients()
    {
        assert (qualityFactor > 0);
        // freq must be smaller than nyquist!
        assert (std::abs(frequency) < currentSampleRate / 2.0);
        if (frequency < 0)
        {
            frequency = std::abs(frequency);
        }
        
        if (algorithm == biquadAlgorithm::LPF)
        {
            
        }
        
        else if (algorithm == biquadAlgorithm::HPF)
        {
            
        }
    }
    
    // RESET
    
    template <typename T>
    void Biquad<T>::reset()
    {
        for (int i = 0; i < zState::numOfZ; ++i)
        {
            zArray[i] = 0; // do I need an explicit cast here?
        }
        
        // what about filterCoefficients array?
    }
    
    template <typename T>
    void Biquad<T>::resetCoefficients()
    {
        for (int i = 0; i < filterCoef::numOfCoef; ++i)
        {
            filterCoefficients[i] = 0; // do I need an explicit cast here?
        }
    }
}
