#pragma once

#include "BinTree.h"

/**
 * @brief 二叉搜索树
 * @tparam T 节点数据类型
 */
template <typename T>
class BinSearchTree : public BinTree<T>
{
public:
    // 在BST中查找关键码e
    virtual BinNodePos<T> &search(const T &e)
    {                                             // 无论命中或失败，hot均指向v之父亲（v是根时，hot为NULL）
        if (!this->root || e == this->root->data) // 空树，或恰在树根命中
        {
            hot = nullptr;
            return this->root;
        }
        for (hot = this->root;;)
        {
            BinNodePos<T> &v = (e < hot->data) ? hot->lChild : hot->rChild;
            if (e == v->data || !v) // 一旦命中或抵达叶子，随即返回
                return v;
            hot = v;
        }
    }
    // 将关键码e插入BST树中
    virtual BinNodePos<T> insert(const T &e)
    {
        BinNodePos<T> &x = search(e);
        if (x) // 如果已经存在e，则不需要插入
            return x;
        x = new BinNode<T>(e, hot);
        this->size++;
        updateHeightAbove(x);
        return x;
    }
    // 删除
    virtual BinNodePos<T> remove(const T &e)
    {
        BinNodePos<T> &x = search(e);
        if (!x) // 如果不存在e，则不需要删除
            return nullptr;
        removeAt(x, hot);
        this->size--;
        updateHeightAbove(x);
        return x;
    }

protected:
    BinNodePos<T> hot; // “命中”节点的父亲
    /**
     * @brief 删除x处的节点
     * @param x 被删除节点
     * @param hot 实际被删除节点的父亲
     * @return BinNodePos<T> 实际被删除节点的接替者
     */
    BinNodePos<T> removeAt(BinNodePos<T> &x, BinNodePos<T> &hot)
    {
        BinNodePos<T> w = x;          // 实际被删除的节点
        BinNodePos<T> succ = nullptr; // 实际被删除节点的接替者
        if (!x->lChild)               // 如果x没有左子树
            succ = x = x->rChild;
        else if (!x->rChild) // 如果右子树为空
            succ = x = x->lChild;
        else // 如果x同时有左右子树，则选择x的直接后继作为实际被摘除节点
        {
            std::swap(x->data, (w = x->succ())->data);
            BinNodePos<T> u = w->parent;
            ((u == x) ? u->rChild : u->lChild) = succ = w->rChild; // 如果w父亲是x，则后继为x的右孩子；反之则为w父亲的左孩子
        }
        hot = w->parent;
        if (succ)
            succ->parent = hot;
        delete w;
    }
};
