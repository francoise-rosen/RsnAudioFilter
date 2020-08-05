/*
  ==============================================================================

    Biquad.h
    Created: 21 Jul 2020 1:15:49pm
    Author:  syfo_dias

  ==============================================================================
*/

/*
 
 Biquad -> FilterDesign -> Filter (biquad, set of biquads)
 
 class Filter that can create different type of filter algos (Biquad, Sequence of Biquads etc.)
 
 Biquad is a self sufficient class, but can be hosted into Filter as well
 
 */

#pragma once

namespace rosen

{
    enum class biquadAlgorithm {LPF, HPF, LPF2, HPF2, ButterLPF2, ButterHPF2, BPF, BSF, HiShelf, LoShelf, numOfAlgorithms};
    
    enum biquadCoeff {a0, a1, a2, b1, b2, numOfCoeff};
    enum zState {x_z1, x_z2, y_z1, y_z2, numOfZ};
    
    //Biquad Class
    template <typename T>
    class Biquad
    {
    public:
        
        Biquad();
        
        Biquad(const T& freq, const T& q, double sampleRate, const unsigned& order = 1, biquadAlgorithm algo = biquadAlgorithm::LPF);
        
        ~Biquad() {}
        
        // setters
        
        void setFrequency(const T& freq);
        void setQ(const T& q);
        void setBiquadAlgorithm(const int&);
        void setSampleRate(const double& sampleRate);
        void setParameters(const T& freq, const T& q, const int& algo);
        
        // getters
        T getFreq() const {return frequency;}
        T getQ() const {return qualityFactor;}
        biquadAlgorithm getAlgorithm() const {return algorithm;}
        
        void resetState();
        void resetCoefficients();
        
        // process
        T process(const T& xn) noexcept;
        //T processBlock(AudioBuffer<T>& buffer) noexcept;
        
        
    private:
        using Math = juce::MathConstants<double>;
        T frequency;
        T qualityFactor;
        double currentSampleRate;
        unsigned filterOrder; // 1 or 2
        biquadAlgorithm algorithm;
        
        //static arrays
        T filterCoefficients[biquadCoeff::numOfCoeff];
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
    Biquad<T>::Biquad(const T& freq, const T& q, double sampleRate, const unsigned& order, biquadAlgorithm algo)
    :frequency{freq}, qualityFactor{q}, currentSampleRate{sampleRate}, filterOrder{order}, algorithm{algo}
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
        setCoefficients(); // does it make sense to invoke this upon every set?
        // alternatively make a struct with all the setters and then as this is changed
        // invoke the setCoefficients() member function
    }
    
    template <typename T>
    void Biquad<T>::setFrequency(const T& freq)
    {
        if (frequency == freq) return;
        frequency = freq;
    
    }
    
    template <typename T>
    void Biquad<T>::setQ(const T& q)
    {
        if (qualityFactor == q) return;
        qualityFactor = q;

    }
    
    template <typename T>
    void Biquad<T>::setBiquadAlgorithm(const int& filterType)
    {
        if (algorithm == (biquadAlgorithm)filterType) return;
        
        if (filterType > (int)(biquadAlgorithm::numOfAlgorithms) - 1)
        {
            algorithm = biquadAlgorithm::LPF;
        }
        else algorithm = (biquadAlgorithm)filterType;

    }
    
    template <typename T>
    void Biquad<T>::setParameters(const T& freq, const T& q, const int& algo)
    {
        if (freq == frequency && q == qualityFactor && algo == (int)algorithm) return;
        
        setFrequency(freq);
        setQ(q);
        setBiquadAlgorithm(algo);
        setCoefficients();
    }
    
    template <typename T>
    void Biquad<T>::setCoefficients()
    {
        assert (qualityFactor > 0);
        assert (filterOrder == 1 || filterOrder == 2);
        // freq must be smaller than nyquist!
        assert (std::abs(frequency) < currentSampleRate / 2.0);
        if (frequency < 0)
        {
            frequency = std::abs(frequency);
        }
        
        // LoPass 1st order
        if (algorithm == biquadAlgorithm::LPF)
        {
            // first order LPF
            T theta = Math::twoPi * frequency / currentSampleRate;
            T gamma = cos(theta) / (1 + sin(theta));
            filterCoefficients[a0] = (1 - gamma) / 2;
            filterCoefficients[a1] = filterCoefficients[a0];
            filterCoefficients[a2] = 0.0;
            filterCoefficients[b1] = -gamma;
            filterCoefficients[b2] = 0.0;
    
            return;
        }
        
        // HiPass 1st order
        if (algorithm == biquadAlgorithm::HPF)
        {
            //first order HPF
            T theta = Math::twoPi * frequency / currentSampleRate;
            T gamma = cos(theta) / (1 + sin(theta));
            filterCoefficients[a0] = (1 + gamma) / 2;
            filterCoefficients[a1] = - filterCoefficients[a0];
            filterCoefficients[a2] = 0.0;
            filterCoefficients[b1] = -gamma;
            filterCoefficients[b2] = 0.0;
            
            return;
        }
        
        if (algorithm == biquadAlgorithm::LPF2)
        {
            T theta = Math::twoPi * frequency / currentSampleRate;
            if (theta >= Math::pi) return; //precaution! but embark on a better check
            
            T d = 1 / qualityFactor;
            
            T beta = 0.5 * (1 - sin(theta) * d * 0.5) / (1 + sin(theta) * d * 0.5);
            T gamma = (0.5 + beta) * cos(theta);
            
            filterCoefficients[a0] = (0.5 + beta - gamma) * 0.5;
            filterCoefficients[a1] = 0.5 + beta - gamma;
            filterCoefficients[a2] = filterCoefficients[a0];
            filterCoefficients[b1] = -2 * gamma;
            filterCoefficients[b2] = 2 * beta;
            
            return;
        }
        
        if (algorithm == biquadAlgorithm::HPF2)
        {
            T theta = Math::twoPi * frequency / currentSampleRate;
            if (theta >= Math::pi) return;
            
            T d = 1 / qualityFactor;
            T beta = 0.5 * (1 - sin(theta) * d * 0.5) / (1 + sin(theta) * d * 0.5);
            T gamma = (0.5 + beta) * cos(theta);
            
            filterCoefficients[a0] = (0.5 + beta + gamma) * 0.5;
            filterCoefficients[a1] = -(0.5 + beta + gamma);
            filterCoefficients[a2] = filterCoefficients[a0];
            filterCoefficients[b1] = -2 * gamma;
            filterCoefficients[b2] = 2 * beta;
            
            return;
        }
        
        // LoPass 2nd order Butterworth
        if (algorithm == biquadAlgorithm::ButterLPF2)
        {
            // ButterWorth LowPass 2nd Order
            //
            T theta = Math::pi * frequency / currentSampleRate;
            if (theta >= Math::pi) return; //precaution! but embark on a better check
            if (theta == Math::halfPi) theta = Math::halfPi + 0.01;
            T mcf = 1 / tan(theta);
            filterCoefficients[a0] = 1 / (1 + mcf * sqrt(2.0) + mcf * mcf);
            filterCoefficients[a1] = 2 * filterCoefficients[a0];
            filterCoefficients[a2] = filterCoefficients[a0];
            filterCoefficients[b1] = 2 * filterCoefficients[a0] * (1 - mcf * mcf);
            filterCoefficients[b2] = filterCoefficients[a0] * (1 - sqrt(2.0) * mcf + mcf * mcf);
            
            return;
        }
        
        // HiPass 2nd order Butterworth
        if (algorithm == biquadAlgorithm::ButterHPF2)
        {
            T theta = Math::pi * frequency / currentSampleRate;
            if (theta >= Math::halfPi) return; //precaution! but embark on a better check
            
            T mcf = tan(theta);
            filterCoefficients[a0] = 1 / (1 + mcf * sqrt(2.0) + mcf * mcf);
            filterCoefficients[a1] = -2 * filterCoefficients[a0];
            filterCoefficients[a2] = filterCoefficients[a0];
            filterCoefficients[b1] = 2 * filterCoefficients[a0] * (mcf * mcf - 1);
            filterCoefficients[b2] = filterCoefficients[a0] * (1 - sqrt(2.0) * mcf + mcf * mcf);
            return;
        }
        
    }
    
    // RESET
    
    template <typename T>
    void Biquad<T>::resetState()
    {
//        for (int i = 0; i < zState::numOfZ; ++i)
//        {
//            zArray[i] = 0; // do I need an explicit cast here?
//        }
        memset(&zArray[0], 0, sizeof(T)*zState::numOfZ);
        
        // what about filterCoefficients array?
    }
    
    template <typename T>
    void Biquad<T>::resetCoefficients()
    {
//        for (int i = 0; i < biquadCoeff::numOfCoef; ++i)
//        {
//            filterCoefficients[i] = 0; // do I need an explicit cast here?
//        }
        
        //memset(&filterCoefficients[0], 0, sizeof(T)*biquadCoeff::numOfCoeff);
        std::fill(&filterCoefficients[0], &filterCoefficients[biquadCoeff::numOfCoeff], 0);
    }
    
    template <typename T>
    T Biquad<T>::process (const T& xn) noexcept
    {
        
        // biquad algorithm
        T yn = filterCoefficients[a0] * xn + filterCoefficients[a1] * zArray[x_z1] + filterCoefficients[a2] * zArray[x_z2] - filterCoefficients[b1] * zArray[y_z1] - filterCoefficients[b2] * zArray[y_z2];
        
        // update registers
        zArray[x_z2] = zArray[x_z1];
        zArray[x_z1] = xn;
        zArray[y_z2] = zArray[y_z1];
        zArray[y_z1] = yn;
        
        return yn;
    }
}
