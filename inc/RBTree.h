#pragma once

#include "inc/BinSearchTree.h"
#include "inc/BinTree.h"
#include "inc/Macro.h"

// 红黑树模板类
template<typename T>
class RBTree : public BinSearchTree<T>
{
public:
    BinNodePos<T> insert(const T& e) override;
    bool          remove(const T& e) override;

protected:
    void solveDoubleRed(BinNodePos<T> x);
    // 解决双黑缺陷
    void solveDoubleBlack(BinNodePos<T> x);
    // 更新黑高度
    Rank updateHeight(BinNodePos<T> x) override
    {
        return x->height = isBlack(x) + std::max(stature(x->l_child), stature(x->r_child));
    }
};

template<typename T>
BinNodePos<T> RBTree<T>::insert(const T& e)
{
    BinNodePos<T>& x = this->search(e);
    if (x != nullptr)
        return x;
    this->size++;
    x = new BinNode<T>(e, this->hot, nullptr, nullptr, 0); // 创造红节点：hot为父，黑高度为0
    BinNodePos<T> x_old = x;
    solveDoubleRed(x);
    return x_old;
}

template<typename T>
bool RBTree<T>::remove(const T& e)
{
    BinNodePos<T> x = this->search(e);
    if (x == nullptr)
        return false;
    BinNodePos<T> r = this->removeAt(x, this->hot); // r:replace, 替换x的节点
    if (--this->size == 0)                          // 删除的是唯一的节点，则直接返回
        return true;
    if (this->hot == nullptr) // 删除的是根节点，则把新根节点染黑，更新黑高度，返回
    {
        this->root->color = RBColor::Black;
        updateHeight(this->root);
        return true;
    }
    if (this->hot->l_child->height == this->hot->r_child->height) // 如果删除后祖先黑高度仍然平衡，则无需调整
    {
        updateHeight(this->hot);
        return true;
    }
    if (isRed(r)) // 否则，此不平衡一定来自于r，所以只需要把r染黑就能解决问题
    {
        r->color = RBColor::Black;
        r->height++;
        return true;
    }
    // solveDoubleBlack(r); // 剩下的情况则是双黑缺陷
    return true;
}

/**
 * @brief 解决双红缺陷
 * @details RR-1：u(uncle)为黑，2次颜色翻转，2次黑高度更新，1~2次旋转，不再递归 \b
 * RR-2：u为红，3次颜色翻转，3次黑高度更新，0次旋转，需要递归
 */
template<typename T>
void RBTree<T>::solveDoubleRed(BinNodePos<T> x)
{
    if (x->isRoot()) // 如果x是树根，则染黑，黑高度++
    {
        this->root->color = RBColor::Black;
        this->root->height++;
        return;
    } // 否则，x的父亲p存在
    BinNodePos<T> p = x->parent;
    if (isBlack(p)) // 如果p是黑色，则不（再）存在双红缺陷
        return;
    BinNodePos<T> g = p->parent; // 如果p为红，则存在祖父g
    BinNodePos<T> u = x->uncle();
    if (isBlack(u)) // RR-1：使用重染色+3+4重构
    { // TODO: 以上虽保证总共两次染色，但因增加了判断而得不偿失，可以改为旋转后三次染色
        (x->isLChild() == p->isLChild()) ? p->color = RBColor::Black :
                                           x->color = RBColor::Black; // 如果x与p同侧，则p染黑；反之x染黑
        g->color                = RBColor::Red;                       // g一定是染红的
        BinNodePos<T>  gg       = g->parent;                          // 曾祖父(great-grandparent)
        BinNodePos<T>& ptr_to_g = this->parentPtrTo(g); // 指向g的指针，接下来要指向新的子树根节点
        ptr_to_g                = this->rotateAt(x);    // 正向链接，指向新的子树根节点
        ptr_to_g->parent        = gg;                   // 反向链接，指向gg
    }
    else // RR-2：递归重染色（等同于节点分裂）
    {
        p->color = u->color = RBColor::Black; // p,u染黑
        p->height++, u->height++;
        g->color = RBColor::Red; // g上升，暂且标记为红
        solveDoubleRed(g); // 继续调整：若已至树根，接下来的递归会将g转黑 // TODO:尾递归，改为迭代
    }
}