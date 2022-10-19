#pragma once

#include "BinSearchTree.h"
#include "inc/BinTree.h"

template<typename T>
class AVLTree : public BinSearchTree<T>
{
    // 下面代码中g==grandparent,p==parent
public:
    virtual BinNodePos<T> insert(const T& e)
    {
        BinNodePos<T>& x = this->search(e);
        if (x) // 如果已经存在e，则不需要插入
            return x;
        x = new BinNode<T>(e, this->hot);
        this->size++;
        // 此时，如果hot增高，则可能失衡
        // for (BinNodePos<T> g = this->hot; g; g = g->parent)
    }
    virtual bool remove(const T& e) {}

protected:
    Rank balanceFactor(BinNodePos<T> x) { stature(x->lChild) - stature(x->rChild); }
    bool avlBalance(BinNodePos<T> x)
    {
        Rank bal_fac = balanceFactor(x);
        return (-1 <= bal_fac) && (bal_fac <= 1);
    }
};