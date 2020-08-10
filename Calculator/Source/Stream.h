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

// TO DO
// make + work. 10+2+= will equal to 12, must be 24 - done

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
void Stream<T>::updateState(const T& value, const Operation& op, bool updateValue, bool updateOperation)
{
    
//    if ((!full) && updateValue)
//    {
//        storedValue = value;
//        full = true;
//        if (updateOperation) operation = op;
//    }
//
//    // there's a value in the buffer
//    // we need
//    else if (full)
//    {
//
//        if (updateValue)
//        {
//            storedValue = value;
//        }
//
//    }
//     if (updateOperation) operation = op;
    if (updateValue && !loop) storedValue = value;
    if (updateOperation)
    {
        operation = op;
        full = true;
    }
}

template <typename T>
T Stream<T>::compute(const T& t)
{
    
    std::cout << "operation: " << operation << '\n';
    T output = t;
    
    if (operation == Operation::plus)
    {
        if (loop)
        {
            storedValue = t;
            loop = false;
            output = storedValue;
        }
        else output = storedValue + t;
        
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
        output = storedValue / t;
        storedValue = t;
    }

    std::cout << "Value In Buffer: " << storedValue << '\n';
 
    return output;
}

template <typename T>
T Stream<T>::compute(Operation op, const T& t)
{
    double output = t;
    if (op == Operation::equals)
    {
        
        if (loop == true)
        {
            if (operation == divide)
            {
                output = t / storedValue;
            }
            else if (operation == minus) output = t - storedValue;
        }
        else output = compute(t);
    }
    
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
T binaryOp(const T& val1, const T& val2, Operation op)
{
    T result = 0;
    if (op == Operation::plus) result = val1 + val2;
    else if (op == Operation::minus) result = val1 - val2;
    else if (op == Operation::multiply) result = val1 * val2;
    else if (op == Operation::divide)
    {
        assert (val2 != 0);
        result = val1 / val2;
    }
    
    return result;
}
