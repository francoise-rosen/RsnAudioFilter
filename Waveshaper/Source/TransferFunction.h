/*
  ==============================================================================

    TransferFunction.h
    Created: 5 Feb 2020 8:13:32pm
    Author:  syfo_dias
 
 https://www.musicdsp.org/en/latest/Effects/42-soft-saturation.html
 
    1. Make TAHN, soft clipping and COS work
    2. How to imlement it in plugin processor? - done, but add parameters
    2.5 More transfer functions (Book)
    3. DCblock
    4. LPG
    5. LPF before shaping
    6. Oversampling
    
    Phase 2
    1. Combination of transfer function (parallel and sequential)

  ==============================================================================
*/

#pragma once


// index range 1.0 - 9.0

// write a scale function

double scale(double valueToScale, double inMix, double inMax, double outMin, double outMax)
{
    double value;
    return value;
}

inline double sgn(double inpt)
{
    return (inpt >= 0) ? 1 : -1;
}

class TransferFunction
{
public:
    TransferFunction()
    {}
    
    enum Functions
    {
        tanh,
        cos,
        sin,
        minPiToPiSin, // how do I implement this?
        softClipper, // index 0.5 - 1
        softClipperCascade3,
        sigmoid1,
        sigmoid2,
        cosTanh,
        TanhCascade3,
        fexp1
        // polynomials?

    };
    
    // TransferFunction(double& signalToTransfer, double index, TransferFunction::functions::function)
    forcedinline double tanh_tfunc(double x, double index) noexcept
    {
        // processing
        auto sample = index * x;
        return std::tanh(sample);
    }
    
    
    //
    forcedinline double softClipper_tfunc(double x, double index) noexcept
    {
        // normalise index of modulation
        double sample = x * index;
        if(std::abs(sample) > 1) sample = sgn(sample) * 2/3;
        if(std::abs(sample) <= -1) sample -= std::pow(sample, 3) / 3;
        
        // normalise
        sample *= 3 / 2;
      
        return sample;
    }
    
    // experiment with coefs here!
    forcedinline double softClipperCascade3f(double x, double index) noexcept
    {
        double sample = softClipper_tfunc(
                            softClipper_tfunc(
                                          softClipper_tfunc(x, index * 0.99998),
                                                                        index * 0.49998),
                                          index * 0.1999998);
        
        return sample;
    };
    
    forcedinline double cos_tfunc(double index, double& x) noexcept
    {
        
        return std::cos(x);
    }
   
    
    forcedinline double transform(Functions func, double index, double& x)
    {
        double sample;
        switch(func)
        {
            case tanh:
                sample = tanh_tfunc(index, x);
                break;
            case cos:
                sample = cos_tfunc(index, x);
                break;
            case softClipper:
                sample = softClipper_tfunc(index, x);
                break;
            default:
                return x;
                
        }
        
        return sample;
    }

private:
    double min;
    double max;
    
};
