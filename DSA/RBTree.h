#pragma once

#include "DSA/BinSearchTree.h"
#include "DSA/BinTree.h"
#include "DSA/Macro.h"

// 红黑树模板类
template<typename T>
class RBTree : public BinSearchTree<T>
{
public:
    BinNodePos<T> insert(const T& e) override;
    bool          remove(const T& e) override;

protected:
    void solveDoubleRed(BinNodePos<T> x);
    void solveDoubleBlack(BinNodePos<T> r);
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
    BinNodePos<T>& x = this->search(e);
    if (x == nullptr)
        return false;
    BinNodePos<T> r = this->removeAt(x); // r:replace, 替换x的节点
    if (--this->size == 0)               // 删除的是唯一的节点，则直接返回
        return true;
    if (this->hot == nullptr) // 删除的是根节点，则把新根节点染黑，更新黑高度，返回
    {
        this->root->color = RBColor::Black;
        updateHeight(this->root);
        return true;
    }
    if (stature(this->hot->l_child) == stature(this->hot->r_child)) // 如果删除后祖先黑高度仍然平衡，则无需调整
    {
        updateHeight(this->hot);
        return true;
    }
    if (isRed(r)) // 否则，此不平衡一定来自于r，但如果r是红色，那么只需要把r染黑就能解决问题
    {
        r->color = RBColor::Black;
        r->height++;
        return true;
    }
    solveDoubleBlack(r); // 剩下的情况则是双黑缺陷
    return true;
}

/**
 * @brief 解决双红缺陷——违反规则3：红节点孩子为黑节点
 * @details
 * RR-1：u(uncle)为黑——2次颜色翻转，2次黑高度更新，1~2次旋转，不再递归
 * RR-2：u为红——3次颜色翻转，3次黑高度更新，0次旋转，需要递归
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
    if (isBlack(u)) // RR-1：使用3+4重构+重染色
    {
        BinNodePos<T>  gg        = g->parent;            // 曾祖父(great-grandparent)
        BinNodePos<T>& ptr_to_g  = this->parentPtrTo(g); // 指向g的指针，接下来要指向新的子树根节点
        ptr_to_g                 = this->rotateAt(x);    // 正向链接，指向新的子树根节点
        ptr_to_g->parent         = gg;                   // 反向链接，指向gg
        ptr_to_g->color          = RBColor::Black;       // 先重构再染色，减少了一次分支判断
        ptr_to_g->l_child->color = ptr_to_g->l_child->color = RBColor::Red;
    }
    else // RR-2：递归重染色（等同于节点分裂）
    {
        p->color = u->color = RBColor::Black; // p,u染黑
        p->height++, u->height++;
        g->color = RBColor::Red; // g上升，暂且标记为红
        solveDoubleRed(g); // 继续调整：若已至树根，接下来的递归会将g转黑 // TODO:尾递归，改为迭代
    }
}

/**
 * @brief 解决双黑缺陷——违反规则4
 * @details
 * BB-1：黑s(sibling)有红子t——2次颜色翻转，2次黑高度更新，1~2次旋转，不再递归
 * BB-2R：黑s无红子，p红——2次颜色翻转，2次黑高度更新，0次旋转，不再递归
 * BB-2B：黑s无红子，p黑——2次颜色翻转，1次黑高度更新，0次旋转，需要递归
 * BB-3：红s——2次颜色翻转，2次黑高度更新，1次旋转，转为BB-1或BB-2R
 */
template<typename T>
void RBTree<T>::solveDoubleBlack(BinNodePos<T> r)
{
    BinNodePos<T> p = r != nullptr ? r->parent : this->hot;
    if (p == nullptr) // 递归到头
        return;
    BinNodePos<T> s = (r == p->l_child) ? p->r_child : p->l_child; // s是r的兄弟，但r可能是外部节点
    if (isBlack(s))
    {
        BinNodePos<T> t = nullptr; // 若左、右孩子皆红，左者优先
        if (isRed(s->r_child))
            t = s->r_child;
        if (isRed(s->l_child))
            t = s->l_child;
        if (t != nullptr) // BB-1：重构——节点旋转
        {
            RBColor        old_color = p->color; // 备份原子树根节点p颜色
            BinNodePos<T>& ptr_to_p  = this->parentPtrTo(p);
            ptr_to_p                 = this->rotateAt(t); // 旋转
            if (ptr_to_p->l_child != nullptr)
            {
                ptr_to_p->l_child->color = RBColor::Black;
                updateHeight(ptr_to_p->l_child);
            }
            if (ptr_to_p->r_child != nullptr)
            {
                ptr_to_p->r_child->color = RBColor::Black;
                updateHeight(ptr_to_p->r_child);
            }
            ptr_to_p->color = old_color; // 新子树根节点继承原根节点的颜色
            updateHeight(ptr_to_p);
        }
        else // BB-2
        {
            s->color = RBColor::Red; // s转红
            s->height--;
            if (isRed(p)) // BB-2R：节点合并，但不会导致上层下溢
                p->color = RBColor::Black;
            else // BB-2B：节点合并，会导致上层下溢
            {
                p->height--;
                solveDoubleBlack(p); // 递归解决上层下溢问题
            }
        }
    }
    else // BB-3：超级节点重染色，需要一次递归
    {
        s->color                = RBColor::Black;
        p->color                = RBColor::Red;
        BinNodePos<T> t         = s->isLChild() ? s->l_child : s->r_child; // 取t与其父s同侧
        this->hot               = p;
        BinNodePos<T>& ptr_to_p = this->parentPtrTo(p);
        ptr_to_p                = this->rotateAt(t); // 对t及其父亲s、祖父p做平衡调整
        solveDoubleBlack(r);                         // 再修正一次
    }
}