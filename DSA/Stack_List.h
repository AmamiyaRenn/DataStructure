#pragma once

#include "List.h"

template<typename T>
class Stack : List<T>
{
public:
    using List<T>::empty;
    using List<T>::List;
    using List<T>::size;
    void push(const T& element) { this->insertAsLast(element); }
    T    pop() { return this->remove(this->last()); }
    T&   top() { return this->last()->element; }
};
