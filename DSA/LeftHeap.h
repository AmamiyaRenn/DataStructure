#pragma once

#include "DSA/BinTree.h"
#include "DSA/Macro.h"
#include "DSA/PriorityQueue.h"
#include <utility>

template<typename T>
class LeftHeap : public PriorityQueue<T>, public BinTree<T>
{
public:
    LeftHeap() = default;
    // 批量构造
    LeftHeap(T* array, Rank n)
    {
        for (Rank i = 0; i < n; i++)
            insert(array[i]);
    }
    // 合并构造
    LeftHeap(LeftHeap& A, LeftHeap B)
    {
        this->root = merge(A.root, B.root);
        this->size = A.size + B.size;
        A.root = B.root = nullptr;
        A.size = B.size = 0;
    }
    void insert(T e) override
    {
        this->root = merge(this->root, new BinNode<T>(e, nullptr));
        this->size++;
    }
    T getMax() const override { return this->root->data; };
    T delMax() override
    {
        BinNodePos<T> l_heap = this->root->l_child;
        if (l_heap != nullptr)
            l_heap->parent = nullptr;
        BinNodePos<T> r_heap = this->root->r_child;
        if (r_heap != nullptr)
            r_heap->parent = nullptr;
        T e = this->root->data;
        delete this->root;
        this->size--;
        this->root = merge(l_heap, r_heap);
        return e;
    }

protected:
    // 合并以a和b为根节点的两个左式堆
    BinNodePos<T> merge(BinNodePos<T> a, BinNodePos<T> b)
    {
        if (a == nullptr)
            return b;
        if (b == nullptr)
            return a;
        if (a->data < b->data)
            std::swap(a, b);                          // 确保a>=b(时刻保证堆序性)
        for (; a->r_child != nullptr; a = a->r_child) // 沿着a的右侧链做二路归并，直到a->r_child先于b变空
            if (a->r_child->data < b->data)
            {
                b->parent = a;
                std::swap(a->r_child, b);
            }
        (a->r_child = b)->parent = a;              // b的残余部分可直接接入
        for (; a != nullptr; b = a, a = a->parent) // 从a的右侧链最后开始回溯(b=a->r_child)
        {
            if (a->l_child == nullptr || a->l_child->npl < a->r_child->npl)
                std::swap(a->l_child, a->r_child);
            a->updateNpl();
        }
        return b; // 返回合并后的堆顶
    }
};
