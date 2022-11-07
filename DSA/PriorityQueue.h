#pragma once

/**
 * @brief 优先级队列ADT
 * @tparam T
 */
template<typename T>
struct PriorityQueue
{
    virtual void insert(T)      = 0;
    virtual T    getMax() const = 0;
    virtual T    delMax()       = 0;
};