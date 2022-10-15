#pragma once

#include "List.h"

template <typename T>
class Queue : protected List<T>
{
public:
    using List<T>::empty;
    using List<T>::List;
    using List<T>::Size;
    void enqueue(const T &e) { this->insertAsLast(e); }
    T dequeue() { return this->remove(this->first()); }
    T &front() { return this->first()->element; }
    T &rear() { return this->lastElement(); }
};
