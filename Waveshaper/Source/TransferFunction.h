/*
  ==============================================================================

    TransferFunction.h
    Created: 5 Feb 2020 8:13:32pm
    Author:  syfo_dias
 
 https://www.musicdsp.org/en/latest/Effects/42-soft-saturation.html
 
    1. Make TAHN and COS work
    2. How to imlement it in plugin processor? - done, but add parameters

  ==============================================================================
*/

#pragma once




class TransferFunction
{
public:
    TransferFunction()
    {}
    
    enum Functions
    {
        tanh,
        cos,
        softClipper
        // polynomials?

    };
    
    // TransferFunction(double& signalToTransfer, double index, TransferFunction::functions::function)
    forcedinline double tanh_tfunc(double index, double& x) noexcept
    {
        // processing
        auto sample = index * x;
        return std::tanh(sample);
    }
    
    
    // improve this algo!!!
    forcedinline double softClipper_tfunc(double index, double& x) noexcept
    {
        // how to implement index here
        auto sample = index * x;
        if( std::abs(sample) < 1) sample -= std::pow(sample, 3) / 3;
        else if( sample >= 1) sample = 2/3;
        else if( sample <= -1) sample = -2/3;
        
        return sample;
    }
    
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
