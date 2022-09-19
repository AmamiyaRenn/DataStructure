#pragma once

#include "List.h"

template <class T>
class Stack : List<T>
{
public:
    using List::List;
    void push(const T &element) { this->insertAsLast(element); }
    T pop() { return this->remove(this->last()); }
    T &top() { return this->lastElement(); }
};
