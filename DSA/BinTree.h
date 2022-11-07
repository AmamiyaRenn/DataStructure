#pragma once

#include "DSA/BinTree.h"
#include "Macro.h"
#include "Queue.h"
#include "Stack.h"

// 红黑树节点颜色
enum class RBColor
{
    Red,
    Black
};

template<typename T>
struct BinNode;

template<typename T>
using BinNodePos = BinNode<T>*;

template<typename T>
struct BinNode
{ // TODO: 重新封装
    T             data;
    BinNodePos<T> parent, l_child, r_child;
    Rank          height;
    Rank          npl;
    RBColor       color; // 红黑树节点颜色
    BinNode() = default;
    explicit BinNode(T             data,
                     BinNodePos<T> parent  = nullptr,
                     BinNodePos<T> l_child = nullptr,
                     BinNodePos<T> r_child = nullptr,
                     Rank          npl     = 1,
                     Rank          height  = 0) :
        data(data),
        parent(parent), l_child(l_child), r_child(r_child), npl(npl), height(height), color(RBColor::Red) {};
    friend Rank stature(BinNodePos<T> x) { return x == nullptr ? -1 : x->height; }
    friend bool isBlack(BinNodePos<T> x) { return x == nullptr || x->color == RBColor::Black; } // 外部节点也算黑节点
    friend bool isRed(BinNodePos<T> x) { return !isBlack(x); }
    void        updateNpl() { npl = r_child != nullptr ? r_child->npl + 1 : 1; }
    // 统计当前节点后代总数，亦即以其为根的子树的规模
    Rank size()
    {
        Rank s = 1;
        if (l_child)
            s += l_child->size();
        if (r_child)
            s += r_child->size();
        return s;
    }
    bool          isRoot() const { return !parent; }
    bool          isLChild() const { return !isRoot() && (this == parent->l_child); }
    bool          isRChild() const { return !isRoot() && (this == parent->r_child); }
    BinNodePos<T> sibling() { return isLChild() ? parent->r_child : parent->l_child; }
    BinNodePos<T> uncle() { return parent->sibling(); }
    // 返回这个节点的直接后继
    BinNodePos<T> succ()
    {
        BinNodePos<T> s = this;
        if (r_child) // 若有右孩子，则直接后继必在右子树中最靠左的节点
        {
            s = r_child;
            while (s->l_child)
                s = s->l_child;
        }
        else // 否则，直接后继应是“将当前节点包含于其左子树中的最低祖先”
        {
            while (s->parent && s == s->parent->r_child)
                s = s->parent;
            s = s->parent;
        }
        return s;
    }
    // 将e作为当前节点的左孩子插入二叉树
    BinNodePos<T> insertAsLChild(const T& e) { return l_child = new BinNode<T>(e, this); }
    // 将e作为当前节点的右孩子插入二叉树
    BinNodePos<T> insertAsRChild(const T& e) { return r_child = new BinNode<T>(e, this); }
    template<typename VST>
    void travPre(VST& visit)
    {
        BinNodePos<T>        x = this;
        Stack<BinNodePos<T>> stack;
        while (true)
        {
            while (x)
            {
                visit(x->data);
                stack.push(x->r_child);
                x = x->l_child;
            }
            if (stack.empty())
                break;
            x = stack.pop();
        }
    }
    template<typename VST>
    void travIn(VST& visit)
    {
        BinNodePos<T>        x = this;
        Stack<BinNodePos<T>> stack;
        while (true)
        {
            while (x)
            {
                stack.push(x);
                x = x->l_child;
            }
            if (stack.empty())
                break;
            visit((x = stack.pop())->data);
            x = x->r_child;
        }
    }
    template<typename VST>
    void travPost(VST& visit)
    {
        BinNodePos<T>        x = this;
        Stack<BinNodePos<T>> stack;
        stack.push(x);
        while (!stack.empty())
        {
            if (stack.top() != x->parent) // 若栈顶非x之父（而为右兄）
            {                             // 在以S栈顶节点为根的子树中，找到最高左侧可见叶节点
                while (BinNodePos<T> p = stack.top())
                    if (p->l_child) // 有左孩子，就深入左边
                    {
                        if (p->r_child)
                            stack.push(p->r_child);
                        stack.push(p->l_child);
                    }
                    else // 实在没办法，就向右找
                        stack.push(p->r_child);
                stack.pop();
            }
            visit((x = stack.pop())->data);
        }
    }
    template<typename VST>
    void travLevel(VST& visit)
    {
        Queue<BinNodePos<T>> queue;
        queue.enqueue(this);
        while (!queue.empty())
        {
            BinNodePos<T> x = queue.dequeue();
            visit(x->data);
            if (x->l_child)
                queue.enqueue(x->l_child);
            if (x->r_child)
                queue.enqueue(x->r_child);
        }
    }
};

template<typename T>
class BinTree
{
public:
    explicit BinTree(Rank size = 0, BinNodePos<T> root = nullptr) : size(size), root(root) {};
    // 层次构造一个完全二叉树
    BinTree(Rank n, T* a);
    ~BinTree() = default;
    Rank          getSize() const { return size; }
    BinNodePos<T> getRoot() const { return root; }
    // 把e作为根节点插入
    BinNodePos<T> insert(const T& e)
    {
        size        = 1;
        return root = new BinNode<T>(e);
    }
    // 把e作为x的左孩子插入，并返回e的位置
    BinNodePos<T> insert(const T& e, BinNodePos<T> x)
    {
        size++;
        x->insertAsLChild(e);
        updateHeightAbove(x);
        return x->l_child;
    }
    // 把e作为x的左孩子插入，并返回e的位置
    BinNodePos<T> insert(BinNodePos<T> x, const T& e)
    {
        size++;
        x->insertAsRChild(e);
        updateHeightAbove(x);
        return x->r_child;
    }
    // 删除二叉树中位置x处的节点及其后代，返回被删除节点的个数
    Rank remove(BinNodePos<T> x)
    {
        parentPtrTo(x) = nullptr;
        updateHeightAbove(x->parent);
        Rank n = removeAt(x);
        size -= n;
        return n;
    }
    // 左子树接入：S当作节点x的左子树接入二叉树，S本身置空，返回x
    BinNodePos<T> attach(BinTree<T>*& S, BinNodePos<T> x)
    {
        if ((x->l_child = S->root))
            x->l_child->parent = x;
        size += S->size;
        updateHeightAbove(x);
        delete S;
        S = nullptr;
        return x;
    }
    // 右子树接入：S当作节点x的右子树接入二叉树，S本身置空，返回x
    BinNodePos<T> attach(BinNodePos<T> x, BinTree<T>*& S)
    {
        if ((x->r_child = S->root))
            x->r_child->parent = x;
        size += S->size;
        updateHeightAbove(x);
        delete S;
        S = nullptr;
        return x;
    }
    // 子树分离：将子树x从当前树中摘除，将其封装为一棵独立子树返回
    BinTree<T> detach(BinNodePos<T> x)
    {
        parentPtrTo(x) = nullptr;
        updateHeightAbove(x->parent);
        x->parent            = nullptr;
        BinTree<T>* new_tree = new BinTree<T>(x->size(), x);
        size -= new_tree->size;
        return new_tree;
    }
    // 前序遍历
    template<typename VST>
    void travPre(VST& visit)
    {
        if (root)
            root->travPre(visit);
    }
    // 中序遍历
    template<typename VST>
    void travIn(VST& visit)
    {
        if (root)
            root->travIn(visit);
    }
    // 后序遍历
    template<typename VST>
    void travPost(VST& visit)
    {
        if (root)
            root->travPost(visit);
    }
    // 层次遍历
    template<typename VST>
    void travLevel(VST& visit)
    {
        if (root)
            root->travLevel(visit);
    }

protected:
    Rank          size;
    BinNodePos<T> root;
    // 返回x的父亲指向x所用的指针
    BinNodePos<T>& parentPtrTo(BinNodePos<T> x)
    {
        return x->isRoot() ? root : (x->isLChild() ? x->parent->l_child : x->parent->r_child);
    }
    // 更新x节点高度
    virtual Rank updateHeight(BinNodePos<T> x)
    {
        return x->height = 1 + std::max(stature(x->l_child), stature(x->r_child));
    }
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
    // 删除二叉树中位置x处的节点及其后代，返回被删除节点的个数
    Rank removeAt(BinNodePos<T> x)
    {
        if (!x)
            return 0;
        Rank n = 1 + removeAt(x->l_child) + removeAt(x->r_child);
        delete x;
        return n;
    }
};