#pragma once

#include "BinTree.h"

/**
 * @brief 二叉搜索树
 * @tparam T 节点数据类型
 */
template<typename T>
class BinSearchTree : public BinTree<T>
{
public:
    // 在BST中查找关键码e
    virtual BinNodePos<T>& search(const T& e)
    { // 无论命中或失败，hot均指向v之父亲（v是根时，hot为NULL）
        if (!this->root || e == this->root->data) // 空树，或恰在树根命中
        {
            hot = nullptr;
            return this->root;
        }
        for (hot = this->root;;)
        {
            BinNodePos<T>& v = (e < hot->data) ? hot->l_child : hot->r_child;
            if (!v || e == v->data) // 一旦抵达叶子或命中，随即返回
                return v;
            hot = v;
        }
    }
    // 将关键码e插入BST树中
    virtual BinNodePos<T> insert(const T& e)
    {
        BinNodePos<T>& x = search(e);
        if (x) // 如果已经存在e，则不需要插入
            return x;
        x = new BinNode<T>(e, hot);
        this->size++;
        this->updateHeightAbove(x);
        return x;
    }
    // 将关键码e从BST树中删除
    virtual bool remove(const T& e)
    {
        BinNodePos<T>& x = search(e);
        if (!x) // 如果不存在e，则不需要删除
            return false;
        removeAt(x, hot);
        this->size--;
        this->updateHeightAbove(x);
        return true;
    }

protected:
    BinNodePos<T> hot; // “命中”节点的父亲
    /**
     * @brief 删除x处的节点
     * @param x 被删除节点
     * @param hot 实际被删除节点的父亲
     * @return BinNodePos<T> 实际被删除节点的接替者
     */
    BinNodePos<T> removeAt(BinNodePos<T>& x, BinNodePos<T>& hot)
    {
        BinNodePos<T> w    = x;       // 实际被删除的节点
        BinNodePos<T> succ = nullptr; // 实际被删除节点的接替者
        if (!x->l_child)              // 如果x没有左子树
            succ = x = x->r_child;
        else if (!x->r_child) // 如果右子树为空
            succ = x = x->l_child;
        else // 如果x同时有左右子树，则选择x的直接后继作为实际被摘除节点
        {
            std::swap(x->data, (w = x->succ())->data);
            BinNodePos<T> u                      = w->parent;
            ((u == x) ? u->r_child : u->l_child) = succ =
                w->r_child; // 如果w父亲是x，则后继为x的右孩子；反之则为w父亲的左孩子
        }
        hot = w->parent;
        if (succ)
            succ->parent = hot;
        delete w;
        return x;
    }
    /**
     * @brief 节点旋转变换
     * @param v 非空孙辈节点
     * @return BinNodePos<T> 调整之后局部节点位置
     */
    BinNodePos<T> rotateAt(BinNodePos<T> v)
    {
        BinNodePos<T> p = v->parent; // parent
        BinNodePos<T> g = p->parent; // grandparent
        if (p->isLChild())           // zig
            if (v->isLChild())       // zig
            {                        // zig-zig
                p->parent = g->parent;
                return connect34(v, p, g, v->l_child, v->r_child, p->r_child, g->r_child);
            }
            else // zag
            {    // zig-zag
                v->parent = g->parent;
                return connect34(p, v, g, p->l_child, v->l_child, v->r_child, g->r_child);
            }
        else                   // zag
            if (v->isLChild()) // zig
            {                  // zag-zig
                v->parent = g->parent;
                return connect34(g, v, p, g->l_child, v->l_child, v->r_child, p->r_child);
            }
            else // zag
            {    // zag-zag
                p->parent = g->parent;
                return connect34(g, p, v, g->l_child, p->l_child, v->l_child, v->r_child);
            }
    }
    /**
     * @brief 根据“3+4”结构连接三个节点与四棵子树
     * @return BinNodePos<T> 重构后局部子树根节点位置(b)
     */
    BinNodePos<T> connect34(BinNodePos<T> a,
                            BinNodePos<T> b,
                            BinNodePos<T> c,
                            BinNodePos<T> T0,
                            BinNodePos<T> T1,
                            BinNodePos<T> T2,
                            BinNodePos<T> T3)
    {
        a->l_child = T0;
        if (T0)
            T0->parent = a;
        a->r_child = T1;
        if (T1)
            T1->parent = a;
        this->updateHeight(a);
        c->l_child = T2;
        if (T2)
            T2->parent = c;
        c->r_child = T3;
        if (T3)
            T3->parent = c;
        this->updateHeight(c);
        b->l_child = a;
        a->parent  = b;
        b->r_child = c;
        c->parent  = b;
        this->updateHeight(b);
        return b;
    }
};
