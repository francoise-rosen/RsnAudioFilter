/*
  ==============================================================================

    Calculator.h
    Created: 28 Jul 2020 4:44:55pm
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once
#include <string>

// rewrite everything using other logic:
// after +,-,/,*,cos,sin,tan pressed the value off the screen will be
// stored in  bufferValue
// the operator will be stored operator = OPERATOR A
// next value entered
// next operator entered (not equals)
// bufferValue = bufferValue OPERATOR B valueOffTheScreen


// PROBLEMS:
// division by 0- returns inf instead of NAN

class Error
{
public:
    Error (const std::string& message, const std::string& display = "")
    :m {message}, d{display} {}
    ~Error() {}
    std::string what() const
    {
        return m;
    }
    std::string show() const
    {
        return d;
    }
private:
    std::string m;
    std::string d;
};


enum Operation {plus, minus, multiply, divide, squareRoot, cosine, sine, tangent, equals, flush, numOfOperators};

template <typename T>
class Stream
{
public:
    Stream();
    ~Stream();
    
    T computeBinary(const T&);
    juce::String computeUnary(const T&, Operation uOp);
    T result(const T&);
    
    void updateBuffer(const T& value, const Operation& op, bool updateValue = true, bool updateOperation = true); // do I need these, or just one function with switch?
    // or use these functions via pointers?

    
    void storeValue(T& value) {storedValue = value;}
    T getValueFromBuffer() const {return storedValue;}
    
    void storeOperator(Operation op) {operation = op;}
    int getOperator() {return static_cast<int>(operation);}
    
    void setFull(bool b) {full = b;}
    bool isFull() const {return full;}
    void reset();
    
private:
    T storedValue;
    Operation operation;
    bool full;
    bool loop;
};

template <typename T>
Stream<T>::Stream()
:storedValue{0}, operation{Operation::plus}, full(false), loop{false}
{}

template <typename T>
Stream<T>::~Stream()
{}

// operations
template <typename T>
void Stream<T>::updateBuffer(const T& value, const Operation& op, bool updateValue, bool updateOperation)
{
    if (updateValue && !loop) storedValue = value;
    if (updateOperation)
    {
        operation = op;
        full = true;
    }
}

template <typename T>
T Stream<T>::computeBinary(const T& t)
{
    
    T output = t;
    if (loop) // previous operator was =
    {
        storedValue = t;
        loop = false;
        output = storedValue;
        return output;
    }
    
    if (operation == Operation::plus)
    {
        
        output = storedValue + t;
        storedValue = t;
        
    }
    else if (operation == Operation::minus)
    {
        output = storedValue - t;
    }
    
    else if (operation == Operation::multiply)
    {
        output = storedValue * t;
    }
    
    else if (operation == Operation::divide)
    {
        if (t == 0)
        {
            throw Error ("Division by zero", "NAN");
        }
        output = storedValue / t;
        storedValue = t;
    }

    std::cout << "Value In Buffer: " << storedValue << '\n';
 
    return output;
}

template <typename T>
juce::String Stream<T>::computeUnary(const T& val, Operation uOp)
{
    juce::String output = unaryOp (val, uOp);
    operation = uOp;
    
    return output;
}

template <typename T>
T Stream<T>::result (const T& t)
{
    T output = t;
    std::cout << "HERE!\n";
    if (loop == true)
    {
        if (operation == divide)
        {
            if(storedValue == 0) return -999;
            output = t / storedValue;
        }
        else if (operation == minus) output = t - storedValue;
        else if (operation == plus) output = t + storedValue;
        else if (operation == multiply) output = t * storedValue;
    }
    else output = computeBinary (t);
    loop = true;
    
    return output;
}


template <typename T>
void Stream<T>::reset()
{
    storedValue = 0;
    operation = Operation::plus;
    full = false;
    loop = false;
}

template <typename T>
juce::String binaryOp(const T& val1, const T& val2, Operation op)
{
    
    T result = 0;
    if (op == Operation::plus)
        result = val1 + val2;
    else if (op == Operation::minus)
        result = val1 - val2;
    else if (op == Operation::multiply)
        result = val1 * val2;
    else if (op == Operation::divide)
    {
        if (val2 == 0)
            return "NAN";
        result = val1 / val2;
    }
    return juce::String(result);
}

template <typename T>
juce::String unaryOp(const T& val, Operation op)
{
    T result = 0;
    if (op == Operation::cosine) result = cos(val);
    else if (op == Operation::sine) result = sin(val);
    else if (op == Operation::tangent) result = tan(val);
    else if (op == Operation::squareRoot)
    {
        if (val < 0) return "NAN";
        result = sqrt(val);
    }
    
    return juce::String(result);
}
