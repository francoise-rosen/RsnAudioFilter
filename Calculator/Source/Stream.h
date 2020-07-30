/*
  ==============================================================================

    Calculator.h
    Created: 28 Jul 2020 4:44:55pm
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once

// for binary operation store a value and an operator
// then enter another value
// and when another binary, unary or = operator pressed
// invoke storedValue, stored operator and the current value using the function compute()

// Problem 1. 30+= will produce 30 + 30 - it's how apple calculator works, but then it'll do
// 60 + 60 etc, must be 60 + 30. Fixed!
// Problem 2. 3 + 3 + 1 = 7 + (and here it'll compute it to 14)

enum Operation {plus, minus, multiply, divide, squareRoot, cosine, sine, tangent, equals, flush, numOfOperators};

template <typename T>
class Stream
{
public:
    Stream();
    ~Stream();
    
    T compute(const T&);
    T compute(Operation op, const T&);
    
    void updateState(const T& value, const Operation& op, bool updateValue = true, bool updateOperation = true); // do I need these, or just one function with switch?
    // or use these functions via pointers?

    
    void storeValue(T& value) {storedValue = value;}
    T getValueFromBuffer() const {return storedValue;}
    
    void storeOperator(Operation op) {operation = op;}
    int getOperator() {return static_cast<int>(operation);}
    
    void setBufferState(bool b) {full = b;}
    bool getBufferState() const {return full;}
    void reset();
    
private:
    
    T storedValue;
    Operation operation;
    bool full;
};

template <typename T>
Stream<T>::Stream()
:storedValue{0}, operation{Operation::equals}, full(false)
{}

template <typename T>
Stream<T>::~Stream()
{}

// operations
template <typename T>
void Stream<T>::updateState(const T& value, const Operation& op, bool updateValue, bool updateOperation)
{
    if (!full && updateValue)
    {
        storedValue = value;
        full = true;
    }
    else if (full)
    {
        if (updateValue)
        {
            storedValue = compute(value); // here change the +,-,/,*
        }
        if (updateOperation) operation = op;
    }
}

template <typename T>
T Stream<T>::compute(const T& t)
{
    T output = storedValue;
    if (operation == Operation::plus)
    {
        output = storedValue + t;
    }
    else if (operation == Operation::minus)
    {
        output = storedValue - t;
    }
    return output;
}

template <typename T>
T Stream<T>::compute(Operation op, const T& t)
{
    
    return t;
}


template <typename T>
void Stream<T>::reset()
{
    storedValue = 0;
    operation = Operation::equals;
}
