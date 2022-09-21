#pragma once

#include "List.h"

template <class T>
class Queue : protected List<T>
{
public:
    using List::empty;
    using List::getSize;
    using List::List;
    void enqueue(const T &e) { this->insertAsLast(e); }
    T dequeue() { return this->remove(this->last()); }
    T &front() { return this->first()->element; }
    T &rear() { return this->lastElement(); }
};
