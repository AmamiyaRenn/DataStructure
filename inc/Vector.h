#ifndef _VECTOR_H
#define _VECTOR_H

#include "Macro.h"
#include "Fibonacci.h"

template <class T>
class Vector
{
public:
    Vector() : capacity(DEFAULT_CAPACITY), size(0), element(nullptr){};
    Vector(const T *element, Rank size) { copyFrom(element, 0, size); }
    Vector(const T *element, Rank low, Rank high) { copyFrom(element, low, high); }
    Vector(const Vector<T> &v) { copyFrom(v.element, 0, v.size); }
    Vector(const Vector<T> &v, Rank low, Rank high) { copyFrom(v.element, low, high); }
    ~Vector() { delete[] this->element; }
    // 重载[]，左值版
    T &operator[](Rank r) { return this->element[r]; }
    // 重载[]，右值版
    T &operator[](Rank r) const { return this->element[r]; }
    // 深复制
    Vector<T> &operator=(const Vector<T> &v)
    {
        if (this->element)
            delete[] this->element;
        copyFrom(v.element, 0, v.size);
        return *this;
    };
    // 输出结构中所有元素
    friend std::ostream &operator<<(std::ostream &os, const Vector &u)
    {
        for (Rank i = 0; i < u.size; i++)
            os << u[i] << " ";
        return os;
    }
    Rank getSize() const { return this->size; }
    bool empty() const { return !this->size; }
    bool full() const { return this->size == this->capacity; }
    // 清空结构
    void clear() { this->size = this->capacity = 0, delete[] this->element, this->element = nullptr; }
    // 判断e是否在结构中
    bool isInside(const T &e) const
    {
        for (Rank i = 0; i < this->size; i++)
            if (e == this->element[i])
                return true;
        return false;
    }
    /**
     * @brief 插入元素
     * @param r 指定秩
     * @param e 元素
     */
    void insert(Rank r, const T &e)
    {
        this->size++;
        expand();
        for (Rank i = this->size; i > r; i--)
            this->element[i] = this->element[i - 1];
        this->element[r] = e;
    }
    // 在末尾插入元素
    void insert(const T &e) { insert(this->size, e); }
    /**
     * @brief 删除[low, high)的元素
     * @param low 左闭
     * @param high 右开
     * @return Rank 被删除的元素个数
     */
    Rank remove(Rank low, Rank high)
    {
        if (low == high)
            return;
        while (high < this->size)
            this->element[low++] = this->element[high++];
        this->size = low;
        shrink();
        return high - low;
    }
    /**
     * @brief 删除单个元素
     * @param r 被删除的元素的秩
     * @return T 被删除的元素
     */
    T remove(Rank r)
    {
        T e = this->element[r];
        remove(r, r + 1);
        return e;
    }
    template <class Opt>
    void traverse(Opt &opt)
    {
        for (Rank i = 0; i < this->size; i++)
            opt(this->element[i]);
    }
    Rank binSearch(const T &e, Rank low, Rank high){};
    Rank fibSearch(const T &e, Rank low, Rank high)
    {
        for (Fibonacci fib(high - low); low < high;)
        {
            while (high - low < fib.get())
                fib.prev();                    // fib(k)永远在[low, high)区间内
            Rank middle = low + fib.get() - 1; // 轴点为fib(k-1)-1
            e < this->element[middle] ? high = middle : low = middle + 1;
        }
        return --low; // 循环结束时，low为大于e的最小秩，所以要--
    }

protected:
    Rank capacity;
    Rank size;
    T *element;

private:
    void copyFrom(const T *element, Rank low, Rank high)
    {
        this->element = new T[this->capacity = max(DEFAULT_CAPACITY, 2 * (high - low))];
        for (this->size = 0; low < high; this->size++, low++)
            this->element[this->size] = element[low]; // 不能使用malloc，因为malloc只能实现浅拷贝
    }
    // 空间扩容
    void expand()
    {
        if (this->size < capacity)
            return;
        if (this->capacity < DEFAULT_CAPACITY)
            this->capacity = DEFAULT_CAPACITY;
        T *oldElement = this->element;
        this->element = new T[capacity <<= 1];
        for (Rank i = 0; i < this->size; i++)
            this->element[i] = oldElement[i];
        delete[] oldElement;
    }
    void shrink()
    {
        if (this->size < DEFAULT_CAPACITY || this->size > this->capacity << 1)
            return;
        T *oldElement = this->element;
        this->element = new T[capacity >>= 1];
        for (Rank i = 0; i < this->size; i++)
            this->element[i] = oldElement[i];
        delete[] oldElement;
    }
};

#endif