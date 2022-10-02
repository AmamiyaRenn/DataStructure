#pragma once

#include "Macro.h"
#include "Stack.h"

template <class T>
struct BinNode;

template <class T>
using BinNodePos = BinNode<T> *;

template <class T>
struct BinNode
{
    T data;
    Rank height;
    BinNodePos<T> parent, lChild, rChild;
    BinNode(){};
    BinNode(T data, BinNodePos<T> xarent = nullptr, BinNodePos<T> lChild = nullptr, BinNodePos<T> rChild = nullptr)
        : data(data), parent(parent), lChild(lChild), rChild(rChild){};
    Rank stature() const { return (*this) ? height : -1; }
    // 将e作为当前节点的左孩子插入二叉树
    BinNodePos<T> insertAsLChild(const T &e) { return lChild = new BinNodePos<T>(e, this); }
    // 将e作为当前节点的右孩子插入二叉树
    BinNodePos<T> insertAsRChild(const T &e) { return rChild = new BinNodePos<T>(e, this); }
    template <typename VST>
    void travPre(VST &visit)
    {
        BinNodePos<T> x = this;
        Stack<BinNodePos<T>> S;
        while (true)
        {
            while (x)
            {
                visit(x->data);
                S.push(x->rChild);
                x = x->lChild;
            }
            if (S.empty())
                break;
            x = S.pop();
        }
    }
    template <typename VST>
    void travIn(VST &visit)
    {
        BinNodePos<T> x = this;
        Stack<BinNodePos<T>> S;
        while (true)
        {
            while (x)
            {
                S.push(x);
                x = x->lChild;
            }
            if (S.empty())
                break;
            visit((x = S.pop())->data);
            x = x->rChild;
        }
    }
    template <typename VST>
    void travPost(VST &visit)
    {
        BinNodePos<T> x = this;
        Stack<BinNodePos<T>> S;
        S.push(x);
        while (!S.empty())
        {
            if (S.top() != x->parent) // 若栈顶非x之父（而为右兄）
            {                         // 在以S栈顶节点为根的子树中，找到最高左侧可见叶节点
                while (BinNodePos<T> p = S.top())
                    if (p->lChild)
                    {
                        if (p->rChild)
                            S.push(p->rChild);
                        S.push(p->lChild);
                    }
                    else
                        S.push(p->lChild);
                S.pop();
            }
            visit((x = S.pop())->data);
        }
    }
    template <typename VST>
    void travLevel(VST &visit)
    {
    }
};

template <class T>
class BinTree
{
public:
    BinTree() : size(0), root(nullptr){};
    // 把e作为根节点插入
    BinNodePos<T> insert(const T &e)
    {
        size = 1;
        return root = new BinNodePos<T>(e);
    }
    // 把e作为x的左孩子插入，并返回e的位置
    BinNodePos<T> insert(const T &e, BinNodePos<T> x)
    {
        size++;
        x->insertAsLChild(e);
        updateHeightAbove(x);
        return x->lChild;
    }
    // 把e作为x的左孩子插入，并返回e的位置
    BinNodePos<T> insert(BinNodePos<T> x, const T &e)
    {
        size++;
        x->insertAsRChild(e);
        updateHeightAbove(x);
        return x->rChild;
    }
    // 前序遍历
    template <typename VST>
    void travPre(VST &visit)
    {
        if (root)
            root->travPre(visit);
    }
    // 中序遍历
    template <typename VST>
    void travIn(VST &visit)
    {
        if (root)
            root->travIn(visit);
    }
    // 后序遍历
    template <typename VST>
    void travPost(VST &visit)
    {
        if (root)
            root->travPost(visit);
    }
    // 层次遍历
    template <typename VST>
    void travLevel(VST &visit)
    {
        if (root)
            root->travLevel(visit);
    }

protected:
    Rank size;
    BinNodePos<T> root;
    // 更新x节点高度
    Rank updateHeight(BinNodePos<T> x) { return x->height = 1 + std::max(x->lChild->stature(), x->rChild->stature()); }
    void updateHeightAbove(BinNodePos<T> x)
    {
        while (x)
        {
            updateHeight(x);
            x = x->parent;
        }
    }
};