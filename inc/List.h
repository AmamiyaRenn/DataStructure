#pragma once

#include "Macro.h"

template <class T>
struct ListNode;

template <class T>
using ListNodePos = ListNode<T> *;

template <class T>
struct ListNode
{
    T element;
    ListNodePos<T> prev, next;
    ListNode(){};
    ListNode(T element, ListNodePos<T> prev = nullptr, ListNodePos<T> next = nullptr) : element(element), prev(prev), next(next){};
    /**
     * @brief 作为当前节点的前一个节点插入
     * @return ListNodePos<T> 新节点位置
     */
    ListNodePos<T> insertAsPrev(const T &element)
    {
        ListNodePos<T> newNode = new ListNode<T>(element, prev, this);
        prev->next = newNode;
        prev = newNode;
        return newNode;
    }
    /**
     * @brief 作为当前节点的后一个节点插入
     * @return ListNodePos<T> 新节点位置
     */
    ListNodePos<T> insertAsNext(const T &element)
    {
        ListNodePos<T> newNode = new ListNode<T>(element, this, next);
        next->prev = newNode;
        next = newNode;
        return newNode;
    }
};

template <class T>
class List
{
public:
    List() { init(); }
    // 把L整个复制过来
    List(const List<T> &L)
    {
        init();
        copyNodes(L.first(), L.getSize());
    }
    // 复制L中自第r项起的n项
    List(const List<T> &L, Rank r, int n)
    {
        init();
        ListNodePos<T> p = L.first();
        while (0 < r--)
            p = p->next;
        copyNodes(p, n);
    }
    ~List() { clear(), delete head, delete tail; }
    T &operator[](Rank r)
    {
        ListNodePos<T> p = first();
        while (0 < r--)
            p = p->next;
        return p->element;
    }
    T &operator[](Rank r) const
    {
        ListNodePos<T> p = first();
        while (0 < r--)
            p = p->next;
        return p->element;
    }
    friend std::ostream &operator<<(std::ostream &os, const List<T> &u)
    {
        ListNodePos<T> p = u.first();
        Rank n = u.getSize();
        while (0 < n--)
        {
            os << p->element << " ";
            p = p->next;
        }
        return os;
    }
    // 首节点位置
    ListNodePos<T> first() const { return head->next; }
    // 尾节点位置
    ListNodePos<T> last() const { return tail->prev; }
    T &firstElement() { return first()->element; }
    T &lastElement() { return last()->element; }
    Rank getSize() const { return size; }
    bool empty() const { return !size; }
    // 在无序列表内的节点p的n个真前驱中找第一个找到的e的位置
    ListNodePos<T> find(const T &e, Rank n, ListNodePos<T> p) const
    {
        while (0 < n--)
            if (e == (p = p->prev)->element)
                return p;
        return nullptr;
    }
    // 在无序列表中找e
    ListNodePos<T> find(const T &e) const { return find(e, size, tail); }
    // 在有序列表内的节点p的n个真前驱中找不大于e的最后者
    ListNodePos<T> search(const T &e, Rank n, ListNodePos<T> p) const
    {
        do
        {
            if (e >= (p = p->prev)->element)
                break;
        } while (0 < n--);
        return p;
    }
    /**
     * @brief 在p前插入e
     * @return ListNodePos<T> e的位置
     */
    ListNodePos<T> insert(const T &e, ListNodePos<T> p)
    {
        size++;
        return p->insertAsPrev(e);
    }
    /**
     * @brief 在p后插入e
     * @return ListNodePos<T> e的位置
     */
    ListNodePos<T> insert(ListNodePos<T> p, const T &e)
    {
        size++;
        return p->insertAsNext(e);
    }
    /**
     * @brief 将e作为首节点插入
     * @return ListNodePos<T> e的位置
     */
    ListNodePos<T> insertAsFirst(const T &e) { return insert(head, e); }
    /**
     * @brief 将e作为尾节点插入
     * @return ListNodePos<T> e的位置
     */
    ListNodePos<T> insertAsLast(const T &e) { return insert(e, tail); }
    /**
     * @brief 删除p处元素
     * @return T p的数据
     */
    T remove(ListNodePos<T> p)
    {
        size--;
        T copy = p->element;
        p->prev->next = p->next;
        p->next->prev = p->prev;
        delete p;
        return copy;
    }
    /**
     * @brief 清空列表
     * @return Rank 原表大小
     */
    Rank clear()
    {
        Rank oldSize = size;
        while (size)
            remove(head->next);
        return oldSize;
    }
    /**
     * @brief 无序列表去重
     * @return Rank 被删除的元素个数
     */
    Rank deduplicate()
    {
        if (size < 2)
            return 0;
        ListNodePos<T> p = first()->next;
        Rank oldSize = size;
        for (Rank r = 1; p != tail; p = p->next)
            if (ListNodePos<T> q = find(p->element, r, p))
                remove(q);
            else
                r++;
        return oldSize - size;
    }
    /**
     * @brief 有序列表去重
     * @return Rank 被删除的元素个数
     */
    Rank uniquify()
    {
        if (size < 2)
            return 0;
        ListNodePos<T> p = first()->next, q;
        Rank oldSize = size;
        while ((q = p->next) != tail)
            if (p->element == q->element)
                remove(q);
            else
                p = q;
        return oldSize - size;
    }
    // 对起始于p后的n个元素进行选择排序
    void selectionSort(ListNodePos<T> p, Rank n)
    {
        ListNodePos<T> sectionHead = p->prev, sectionTail = p;
        for (int i = 0; i < n; i++)
            sectionTail = sectionTail->next;
        for (; 1 < n; n--)
        {
            insert(remove(selectMax(sectionHead->next, n)), sectionTail);
            sectionTail = sectionTail->prev;
        }
    }
    //对列表中起始于位置p、宽度为n的区间做插入排序
    void insertionSort(ListNodePos<T> p, Rank n)
    {
        for (Rank r = 0; r < n; r++)
        {
            insert(search(p->element, r, p), p->element);
            remove((p = p->next)->prev);
        }
    }
    // p后n个元素内找到最大的元素返回其位置
    ListNodePos<T> selectMax(ListNodePos<T> p, Rank n)
    {
        ListNodePos<T> max = p;
        for (; 1 < n; n--)
            if (max->element <= (p = p->next)->element)
                max = p;
        return max;
    }

protected:
    Rank size;
    ListNodePos<T> head, tail;

private:
    void init()
    {
        size = 0;
        head = new ListNode<T>(), tail = new ListNode<T>();
        head->prev = nullptr, head->next = tail, tail->prev = head, tail->next = nullptr;
    }
    void copyNodes(ListNodePos<T> p, Rank n)
    {
        while (n--)
            insertAsLast(p->element), p = p->next;
    }
};
