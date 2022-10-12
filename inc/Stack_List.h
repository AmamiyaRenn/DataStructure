#pragma once

#include "List.h"

template <typename T>
class Stack : List<T>
{
public:
    using List::empty;
    using List::List;
    using List::Size;
    void push(const T &element) { this->insertAsLast(element); }
    T pop() { return this->remove(this->last()); }
    T &top() { return this->last()->element; }
};
