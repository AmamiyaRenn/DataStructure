#pragma once

#include "Vector.h"

template<typename T>
class Stack : Vector<T>
{
public:
    using Vector<T>::empty;
    using Vector<T>::size;
    using Vector<T>::Vector;
    void push(const T& element) { this->insert(element); }
    T    pop() { return this->remove(this->size - 1); }
    T&   top() { return (*this)[this->size - 1]; }
};
