#pragma once

#include "DSA/Macro.h"
#include "DSA/PriorityQueue.h"
#include "DSA/Vector.h"

/**
 * @brief 完全二叉堆
 * @tparam T
 */
template<typename T>
class CompleteHeap : public PriorityQueue<T>, public Vector<T>
{
public:
    CompleteHeap() = default;
    CompleteHeap(T* array, Rank n)
    {
        Vector<T>::copyFrom(array, 0, n);
        heapify(this->element, n);
    }
    void insert(T e) override
    {
        Vector<T>::insert(e);
        percolateUp(this->element, this->size - 1);
    }
    T getMax() const override { return this->element[0]; }
    T delMax() override
    {
        T max_elem       = this->element[0];
        this->element[0] = this->element[--this->size];
        percolateDown(this->element, this->size, 0);
        return max_elem;
    }

protected:
    Rank inHeap(Rank n, Rank i) const { return -1 < i && i < n; }
    Rank parent(Rank i) { return (i - 1) >> 1; }
    Rank lChild(Rank i) { return (i << 1) + 1; }
    Rank rChild(Rank i) { return (i << 1) + 2; }
    Rank lChildValid(Rank n, Rank i) { return inHeap(n, lChild(i)); }
    Rank rChildValid(Rank n, Rank i) { return inHeap(n, rChild(i)); }
    // 父子（至多）三者中的最大者
    Rank properParent(T* array, Rank n, Rank i)
    {
        auto bigger = [&](Rank i, Rank j) { return array[i] < array[j] ? j : i; }; // 取秩大者
        return rChildValid(n, i) ? bigger(i, bigger(lChild(i), rChild(i))) :
                                   (lChildValid(n, i) ? bigger(i, lChild(i)) : i);
    }
    /**
     * @brief 对向量前n个词条的第i个实施下滤
     * @return Rank 下滤抵达的位置
     */
    Rank percolateDown(T* array, Rank n, Rank i)
    {
        Rank p; // p is the parent of i
        while (i != (p = properParent(array, n, i)))
        {
            std::swap(array[i], array[p]); // TODO:可改进为赋值
            i = p;
        }
        return i;
    }
    Rank percolateUp(T* array, Rank i)
    {
        while (0 < i) // 抵达顶点前
        {
            Rank p = parent(i);
            if (array[i] < array[p])
                break; // 堆序性已经满足
            std::swap(array[i], array[p]);
            i = p;
        }
        return i;
    }
    /**
     * @brief Floyd建堆算法
     * @param array 向量
     * @param n 待建堆元素数
     */
    void heapify(T* array, Rank n)
    {
        for (Rank i = n / 2 - 1; 0 <= i; i--) // 自下而上的下滤
            percolateDown(array, n, i);
    }
};
