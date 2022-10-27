#pragma once

#include "BinSearchTree.h"
#include "inc/BinTree.h"
#include <sys/types.h>
#include <type_traits>

template<typename T>
class AVLTree : public BinSearchTree<T>
{
public:
    /**
     * @brief 插入关键码
     * @param e 关键码
     * @return BinNodePos<T> 新节点位置
     */
    virtual BinNodePos<T> insert(const T& e);
    /**
     * @brief 插入关键码
     * @param e 关键码
     * @return true 删除成功
     * @return false 删除失败
     */
    virtual bool remove(const T& e);

protected:
    // 平衡因子
    Rank balanceFactor(BinNodePos<T> x) { return stature(x->l_child) - stature(x->r_child); }
    // AVLTree平衡条件
    bool avlBalance(BinNodePos<T> x)
    {
        Rank bal_fac = balanceFactor(x);
        return (-1 <= bal_fac) && (bal_fac <= 1);
    }
    // x的左右孩子中选更高者
    BinNodePos<T> tallerChild(BinNodePos<T> x)
    {
        Rank bal_fac = balanceFactor(x);
        if (bal_fac == 0)
            return x->isLChild() ? x->l_child : x->r_child; // 等高则与x同侧(zig-zig,zag-zag)优先
        return bal_fac > 0 ? x->l_child : x->r_child;
    }
};

template<typename T>
BinNodePos<T> AVLTree<T>::insert(const T& e)
{
    BinNodePos<T>& x = this->search(e);
    if (x) // 如果已经存在e，则不需要插入
        return x;
    x = new BinNode<T>(e, this->hot);
    this->size++;
    // 此时，如果hot增高，则可能失衡
    for (BinNodePos<T> g = this->hot; g; g = g->parent)
        if (!avlBalance(g))
        {
            BinNodePos<T>& p_ptr_to_g = this->parentPtrTo(g);
            p_ptr_to_g                = this->rotateAt(tallerChild(tallerChild(g)));
            break; // 局部子树恢复后，高度必然复原
        }
        else
            this->updateHeight(g);
    return x;
}

template<typename T>
bool AVLTree<T>::remove(const T& e)
{
    BinNodePos<T>& x = this->search(e);
    if (!x) // 如果已经存在e，则不需要插入
        return false;
    this->removeAt(x, this->hot);
    this->size--;
    for (BinNodePos<T> g = this->hot; g; g = g->parent)
    {
        if (!avlBalance(g))
        {
            BinNodePos<T>& ptr = this->parentPtrTo(g);
            ptr                = this->rotateAt(tallerChild(tallerChild(g)));
        }
        this->updateHeight(g); // 一次调整可能导致全树高度降低
    }
    return true;
}