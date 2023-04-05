#pragma once

#include "BinSearchTree.h"

/**
 * @brief 伸展树
 * @tparam T 节点数据类型
 */
template<typename T>
class SplayTree : public BinSearchTree<T>
{
public:
    // 查找
    BinNodePos<T>& search(const T& e)
    {
        BinNodePos<T> p = BinSearchTree<T>::search(e);
        this->root      = splay(p == nullptr ? this->hot : p);
        return this->root;
    }
    // 插入
    BinNodePos<T> insert(const T& e);
    // 删除
    bool remove(const T& e);

protected:
    /**
     * @brief 将节点v伸展至根
     * @return BinNodePos<T> v(root)
     */
    BinNodePos<T> splay(BinNodePos<T>& v);

private:
    // 把lc变成p左孩子
    void attachAsLC(BinNodePos<T> lc, BinNodePos<T> p)
    {
        p->l_child = lc;
        if (lc)
            lc->parent = p;
    }
    // 把rc变成p右孩子
    void attachAsRC(BinNodePos<T> p, BinNodePos<T> rc)
    {
        p->r_child = rc;
        if (rc)
            rc->parent = p;
    }
};

template<typename T>
BinNodePos<T> SplayTree<T>::insert(const T& e)
{
    if (this->root == nullptr) // 原树为空
    {
        this->size        = 1;
        return this->root = new BinNode<T>(e);
    }
    BinNodePos<T> t = search(e);
    if (e == t->data) // 节点存在
        return t;
    if (t->data < e) // 在右侧嫁接
    {
        t->parent = this->root = new BinNode<T>(e, nullptr, t, t->r_child);
        if (t->r_child != nullptr)
        {
            t->r_child->parent = this->root;
            t->r_child         = nullptr;
        }
    }
    else
    {
        t->parent = this->root = new BinNode<T>(e, nullptr, t->l_child, t);
        if (t->l_child != nullptr)
        {
            t->l_child->parent = this->root;
            t->l_child         = nullptr;
        }
    }
    this->size++;
    this->updateHeightAbove(t);
    return this->root;
}
template<typename T>
bool SplayTree<T>::remove(const T& e)
{
    if (this->root == nullptr || e != search(e)->data)
        return false;
    BinNodePos<T> root_l = this->root->l_child;
    BinNodePos<T> root_r = this->root->r_child;
    delete this->root;
    if (root_r == nullptr)
    {
        if (root_l != nullptr) // 不存在右子树但存在左子树
            root_l->parent = nullptr;
        this->root = root_l;
    }
    else
    {
        this->root     = root_r;
        root_r->parent = nullptr;
        search(e);             // 在R中再次查找e：注定失败，但其中的最小节点必成为根节点
        if (root_l != nullptr) // 嫁接左子树
            root_l->parent = this->root;
        this->root->l_child = root_l;
    }
    if (--this->size == 0)
        this->updateHeight(this->root);
    return true;
}

template<typename T>
BinNodePos<T> SplayTree<T>::splay(BinNodePos<T>& v)
{
    if (v == nullptr)
        return nullptr;
    BinNodePos<T> p;                           // v's parent;
    BinNodePos<T> g;                           // v's grandparent;
    while ((p = v->parent) && (g = p->parent)) // 双层伸展
    {
        BinNodePos<T> gg = g->parent; // 每轮后v都以原曾祖父(great-grandparent)为父
        if (v->isLChild())
        {
            if (p->isLChild())
            { // zig-zig
                attachAsLC(p->r_child, g);
                attachAsLC(v->r_child, p);
                attachAsRC(p, g);
                attachAsRC(v, p);
            }
            else
            { // zig-zag
                attachAsLC(v->r_child, p);
                attachAsRC(g, v->l_child);
                attachAsLC(g, v);
                attachAsRC(v, p);
            }
        }
        else
        {
            if (p->isLChild())
            { // zag-zig
                attachAsRC(p, v->l_child);
                attachAsLC(v->r_child, g);
                attachAsRC(v, g);
                attachAsLC(p, v);
            }
            else
            { // zag-zag
                attachAsRC(g, p->l_child);
                attachAsRC(p, v->l_child);
                attachAsLC(g, p);
                attachAsLC(p, v);
            }
        }
        if (gg == nullptr) // gg不存在，则v现在是树根
            v->parent = nullptr;
        else // 否则，v当为gg的左或右孩子
            (g == gg->l_child) ? attachAsLC(v, gg) : attachAsRC(gg, v);
        this->updateHeight(g);
        this->updateHeight(p);
        this->updateHeight(v);
    }
    if ((p = v->parent) != nullptr) // 如果双层伸展结束后v还没到顶，说明还需要单层伸展
    {
        if (v->isLChild())
        {
            attachAsLC(v->r_child, p);
            attachAsRC(v, p);
        }
        else
        {
            attachAsRC(p, v->l_child);
            attachAsLC(p, v);
        }
        this->updateHeight(p);
        this->updateHeight(v);
    }
    v->parent = nullptr;
    return v;
}