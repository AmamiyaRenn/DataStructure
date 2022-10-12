#pragma once

#include "List.h"

template <typename T>
class Queue : protected List<T>
{
public:
    using List::empty;
    using List::List;
    using List::Size;
    void enqueue(const T &e) { this->insertAsLast(e); }
    T dequeue() { return this->remove(this->first()); }
    T &front() { return this->first()->element; }
    T &rear() { return this->lastElement(); }
};
