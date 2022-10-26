#pragma once

#include "inc/Macro.h"
#include "inc/Queue.h"
#include "inc/Vector.h"
#include <iostream>

template<typename T>
struct BTNode;
template<typename T>
using BTNodePos = BTNode<T>*; // B-树节点位置

// B-树节点模板类
template<typename T>
struct BTNode
{
    BTNodePos<T>         parent; // 父节点
    Vector<T>            key;    // 关键码向量
    Vector<BTNodePos<T>> child;  // 孩子向量
    BTNode() : parent(nullptr) { child.insert(nullptr); }
    explicit BTNode(T e, BTNodePos<T> lChild = nullptr, BTNodePos<T> rChild = nullptr) : parent(nullptr)
    {
        key.insert(e); // 作为根节点只有一个关键码与（可能存在的）一个左孩子与一个右孩子
        child.insert(lChild);
        if (lChild != nullptr)
            lChild->parent = this;
        child.insert(rChild);
        if (rChild != nullptr)
            rChild->parent = this;
    }
};

// B-树模板类
template<typename T>
class BTree
{
public:
    explicit BTree(Rank order = 5) : size(0), order(order), root(new BTNode<T>) {}
    ~BTree() { delete root; }
    Rank          getSize() const { return size; }
    Rank          getOrder() const { return order; }
    BTNodePos<T>& getRoot() { return root; }
    bool          empty() const { return root == nullptr; }
    BTNodePos<T>  search(const T& e);
    bool          insert(const T& e);
    bool          remove(const T& e);
    void          print()
    {
        BTNodePos<T>        v;
        auto                fn = [](T& e) { std::cout << e << " "; };
        Queue<BTNodePos<T>> q;
        q.enqueue(root);
        auto enq = [&](BTNodePos<T>& e) {
            if (e != nullptr)
                q.enqueue(e);
        };
        while (!q.empty())
        {
            v = q.dequeue();
            v->key.traverse(fn);
            v->child.traverse(enq);
            std::cout << "\n";
        }
    }

protected:
    Rank         size;  // 关键码总数
    Rank         order; // 阶数
    BTNodePos<T> root;
    BTNodePos<T> hot;
    // 通过节点分裂，解决上溢问题
    void solveOverflow(BTNodePos<T> v);
    // 通过节点旋转或节点合并，解决下溢问题
    void solveUnderflow(BTNodePos<T> v);
};

template<typename T>
BTNodePos<T> BTree<T>::search(const T& e)
{
    BTNodePos<T> v = root;
    hot            = nullptr; // 从root开始
    while (v != nullptr)      // 逐层查找
    {
        Rank r = v->key.search(e); // 找到当前节点中不大于e的最大关键码，二分查找(可能效率比不过逐个查找)
        if ((0 <= r) && (e == v->key[r]))
            return v; // 查找成功
        hot = v;
        v   = v->child[r + 1]; // 否则，进入对应子树
    }
    return nullptr; // 失败
}

template<typename T>
bool BTree<T>::insert(const T& e)
{
    BTNodePos<T> v = search(e);
    if (v != nullptr)
        return false;
    Rank r = hot->key.search(e); // 插入e的合适位置
    hot->key.insert(r + 1, e);
    hot->child.insert(r + 2, nullptr);
    size++;
    solveOverflow(hot);
    return true;
}

template<typename T>
bool BTree<T>::remove(const T& e)
{
    BTNodePos<T> v = search(e);
    if (v == nullptr)
        return false;
    Rank r = v->key.search(e);  // e的秩
    if (v->child[0] != nullptr) // 如果v不是叶子，则v后继一定是叶子
    {
        BTNodePos<T> u = v->child[r + 1]; // 找右子树最左
        while (u->child[0] != nullptr)
            u = u->child[0];   // 至此，u必然到达叶子
        v->key[r] = u->key[0]; // 通过简单的替换完成节点删除
        v         = u;         // 此时u需要删除，借助v
        r         = 0;
    }
    v->key.remove(r);
    v->child.remove(r + 1);
    size--;
    solveUnderflow(v);
}

template<typename T>
void BTree<T>::solveOverflow(BTNodePos<T> v)
{
    while (order <= v->key.getSize()) // 上溢条件：order<=节点关键码个数
    {
        Rank pivot = order / 2; // 分裂的轴点ks
        // 分出v, u
        BTNodePos<T> u = new BTNode<T>; // [ks+1, order)
        for (Rank i = 0; i < order - pivot - 1; i++)
        { // TODO:下面是两句效率奇低的移动策略，需要改进
            u->child.insert(i, v->child.remove(pivot + 1));
            u->key.insert(i, v->key.remove(pivot + 1));
        } // v此时变为了[0, ks]
        // u->child.insert(v->child.remove(pivot + 1)); // 补充child的最后一个插入
        u->child[order - pivot - 1] = v->child.remove(pivot + 1);
        if (u->child[0] != nullptr) // 如果u孩子非空，则令它们父节点为u
        {
            // auto fn = [&](BTNodePos<T>& e) { e->parent = u; };
            // u->child.traverse(fn);
            for (Rank i = 0; i < order - pivot; i++)
                u->child[i]->parent = u;
        }
        // 处理p
        BTNodePos<T> p = v->parent; // v的父节点p
        if (p == nullptr)           // 如果p为空则创建之
        {
            root = p    = new BTNode<T>;
            p->child[0] = v;
            v->parent   = p;
        }
        Rank r = 1 + p->key.search(v->key[0]);  // 轴点关键码上升后的位置
        p->key.insert(r, v->key.remove(pivot)); // 轴点关键码上升，此时v为[0, ks)
        p->child.insert(r + 1, u);
        u->parent = p; // 新节点u与父节点互联
        v         = p; // v上升一层
    }
}

template<typename T>
void BTree<T>::solveUnderflow(BTNodePos<T> v)
{
    while (v->child.getSize() < (order + 1) / 2) // 下溢条件：节点关键码个数<Ceiling(order/2)
    {
        BTNodePos<T> p = v->parent; // v的父节点p
        if (p == nullptr)
        { // v已经到达根节点
            if (v->key.getSize() && v->child[0] != nullptr)
            { // 此时v如果不含有关键码却有非空孩子(一定唯一)，则需要删除这一层
                root         = v->child[0]; // 根节点转交
                root->parent = nullptr;
                delete v; // 删除层
            }
            return;
        }
        Rank r = p->child.find(v);
    }
}