#pragma once

#include "Macro.h"
#include "Stack.h"

template <typename T>
struct BinNode;

template <typename T>
using BinNodePos = BinNode<T> *;

template <typename T>
struct BinNode
{
    T data;
    BinNodePos<T> parent, lChild, rChild;
    Rank height;
    BinNode(){};
    BinNode(T data, BinNodePos<T> parent = nullptr, BinNodePos<T> lChild = nullptr, BinNodePos<T> rChild = nullptr, Rank height = 0)
        : data(data), parent(parent), lChild(lChild), rChild(rChild), height(0){};
    friend Rank stature(BinNodePos<T> x) { return x == nullptr ? -1 : x->height; }
    // 统计当前节点后代总数，亦即以其为根的子树的规模
    Rank size()
    {
        Rank s = 1;
        if (lChild)
            s += lChild->size();
        if (rChild)
            s += rChild->size();
        return s;
    }
    // 返回x的父亲指向x所用的指针
    BinNodePos<T> &ParentPtrToThis() { return parent != nullptr ? (parent->lChild == this ? parent->lChild : parent->rChild) : parent; }
    // 返回这个节点的直接后继
    BinNodePos<T> succ()
    {
        BinNodePos<T> s = this;
        if (rChild) // 若有右孩子，则直接后继必在右子树中最靠左的节点
        {
            s = rChild;
            while (s->lChild)
                s = s->lChild;
        }
        else // 否则，直接后继应是“将当前节点包含于其左子树中的最低祖先”
        {
            while (s->parent && s == s->parent->rChild)
                s = s->parent;
            s = s->parent;
        }
        return s;
    }
    // 将e作为当前节点的左孩子插入二叉树
    BinNodePos<T> insertAsLChild(const T &e) { return lChild = new BinNode<T>(e, this); }
    // 将e作为当前节点的右孩子插入二叉树
    BinNodePos<T> insertAsRChild(const T &e) { return rChild = new BinNode<T>(e, this); }
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
                    if (p->lChild) // 有左孩子，就深入左边
                    {
                        if (p->rChild)
                            S.push(p->rChild);
                        S.push(p->lChild);
                    }
                    else // 实在没办法，就向右找
                        S.push(p->rChild);
                S.pop();
            }
            visit((x = S.pop())->data);
        }
    }
    template <typename VST>
    void travLevel(VST &visit)
    {
        Queue<BinNodePos<T>> Q;
        Q.enqueue(this);
        while (!Q.empty())
        {
            BinNodePos<T> x = Q.dequeue();
            visit(x->data);
            if (x->lChild)
                Q.enqueue(x->lChild);
            if (x->rChild)
                Q.enqueue(x->rChild);
        }
    }
};

template <typename T>
class BinTree
{
public:
    BinTree(Rank size = 0, BinNodePos<T> root = nullptr) : size(size), root(root){};
    // 层次构造一个完全二叉树
    BinTree(Rank n, T *a)
    {
        Queue<BinNodePos<T>> Q;
        insert(a[0]);
        Q.enqueue(root);
        for (Rank i = 1; i < n - 1; i += 2)
        {
            BinNodePos<T> node = Q.dequeue();
            insert(a[i], node);
            Q.enqueue(node->lChild);
            insert(node, a[i + 1]);
            Q.enqueue(node->rChild);
        }
        if (size != n)
            insert(a[n - 1], Q.dequeue());
    }
    ~BinTree(){};
    Rank Size() const { return size; }
    BinNodePos<T> Root() const { return root; }
    // 把e作为根节点插入
    BinNodePos<T> insert(const T &e)
    {
        size = 1;
        return root = new BinNode<T>(e);
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
    // 删除二叉树中位置x处的节点及其后代，返回被删除节点的数值
    Rank remove(BinNodePos<T> x)
    {
        x->ParentPtrToThis() = nullptr;
        updateHeightAbove(x->parent);
        Rank n = removeAt(x);
        size -= n;
        return n;
    }
    // 左子树接入：S当作节点x的左子树接入二叉树，S本身置空，返回x
    BinNodePos<T> attach(BinTree<T> *&S, BinNodePos<T> x)
    {
        if ((x->lChild = S->root))
            x->lChild->parent = x;
        size += S->size;
        updateHeightAbove(x);
        delete S;
        S = nullptr;
        return x;
    }
    // 右子树接入：S当作节点x的右子树接入二叉树，S本身置空，返回x
    BinNodePos<T> attach(BinNodePos<T> x, BinTree<T> *&S)
    {
        if ((x->rChild = S->root))
            x->rChild->parent = x;
        size += S->size;
        updateHeightAbove(x);
        delete S;
        S = nullptr;
        return x;
    }
    // 子树分离：将子树x从当前树中摘除，将其封装为一棵独立子树返回
    BinTree<T> detach(BinNodePos<T> x)
    {
        x->ParentPtrToThis() = nullptr;
        updateHeightAbove(x->parent);
        x->parent = nullptr;
        BinTree<T> *S = new BinTree<T>(x->size(), x);
        size -= S->size;
        return S;
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
    Rank updateHeight(BinNodePos<T> x) { return x->height = 1 + std::max(stature(x->lChild), stature(x->rChild)); }
    // 更新x节点及其所有祖先高度
    void updateHeightAbove(BinNodePos<T> x)
    {
        while (x)
        {
            updateHeight(x);
            x = x->parent;
        }
    }

private:
    // 删除二叉树中位置x处的节点及其后代，返回被删除节点的数值
    Rank removeAt(BinNodePos<T> x)
    {
        if (!x)
            return 0;
        Rank n = 1 + removeAt(x->lChild) + removeAt(x->rChild);
        delete x;
        return n;
    }
};