/*
  ==============================================================================

    TransferFunction.h
    Created: 5 Feb 2020 8:13:32pm
    Author:  syfo_dias
 
 https://www.musicdsp.org/en/latest/Effects/42-soft-saturation.html
 
    1. Make TAHN, soft clipping and COS work
    2. How to imlement it in plugin processor? - done, but add parameters
    2.5 More transfer functions (Book)
    3. DCblock!
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

// do I need these const here?
template <typename T>
T linearScale(T& valueToScale, const T inMin, const T inMax, const T outMin, const T outMax)
{
    T value;
    jassert(inMin < inMax);
    if(outMin == outMax) return outMin;
    
    const T factor = (outMax - outMin) / (inMax - inMin);
    value = factor * (valueToScale - inMin) + outMin;
    
    return value;
}

template<typename T>
T expScale(T& valueToScale, const T inMin, const T inMax, const T outMin, const T outMax, const T expFactor)
{
    // expFactor in range (1, n) will produce exponential growth
    // expFactor in range (0, 1) will produce logarithmic growth
    // expFactor 1 is simple linearScale;
    jassert(expFactor >= 0);
    jassert(valueToScale >= 0); // we don't need negative numbers here (problems woth sqrt())
    
    T value = valueToScale;
    if(expFactor == static_cast<T>(1)) return linearScale(valueToScale, inMin, inMax, outMin, outMax);
    
    // assume the input range is 0 - 1 (otherwise we may need to scale it first
    if((inMin == static_cast<T>(0) && inMax == static_cast<T>(1)))
    {
        value = pow(value, expFactor);
    }
    else
    {
        T localMin = static_cast<T>(0);
        T localMax = static_cast<T>(1);
        value = linearScale(value, inMin, inMax, localMin, localMax);
        value = pow(value, expFactor);
    }
    
    value = linearScale(value, static_cast<T>(0), static_cast<T>(1), outMin, outMax);
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
        negPiToPiSin, // how do I implement this? scale input to - pi/2, pi/2
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
