#ifndef _VECTOR_H
#define _VECTOR_H

#include "Macro.h"
#include "Fibonacci.h"

template <typename T>
class Vector
{
public:
    Vector(const Rank capacity = DEFAULT_CAPACITY, Rank size = 0)
        : capacity(capacity), size(size), element(new T[capacity]){};
    Vector(T element = T(), const Rank capacity = DEFAULT_CAPACITY, Rank size = 0)
        : capacity(capacity), size(size), element(new T[capacity])
    {
        for (int i = 0; i < size; i++)
            this->element[i] = element;
    };
    Vector(const T *element, Rank size) { copyFrom(element, 0, size); }
    Vector(const T *element, Rank low, Rank high) { copyFrom(element, low, high); }
    Vector(const Vector<T> &v) { copyFrom(v.element, 0, v.size); }
    Vector(const Vector<T> &v, Rank low, Rank high) { copyFrom(v.element, low, high); }
    ~Vector() { delete[] element; }
    // 重载[]，左值版
    T &operator[](Rank r) { return element[r]; }
    // 重载[]，右值版
    T &operator[](Rank r) const { return element[r]; }
    // 深复制
    Vector<T> &operator=(const Vector<T> &v)
    {
        if (element)
            delete[] element;
        copyFrom(v.element, 0, v.size);
        return *this;
    };
    Rank Size() const { return size; }
    bool empty() const { return !size; }
    bool full() const { return size == capacity; }
    // 清空结构
    void clear() { size = capacity = 0, delete[] element, element = nullptr; }
    // 判断e是否在结构中
    bool isInside(const T &e) const
    {
        for (Rank i = 0; i < size; i++)
            if (e == element[i])
                return true;
        return false;
    }
    /**
     * @brief 插入元素
     * @param r 指定秩
     * @param e 元素
     */
    Rank insert(Rank r, const T &e)
    {
        expand();
        for (Rank i = size; i > r; i--)
            element[i] = element[i - 1];
        element[r] = e;
        size++;
        return r;
    }
    // 在末尾插入元素
    Rank insert(const T &e) { return insert(size, e); }
    /**
     * @brief 删除[low, high)的元素
     * @param low 左闭
     * @param high 右开
     * @return Rank 被删除的元素个数
     */
    Rank remove(Rank low, Rank high)
    {
        if (low == high)
            return 0;
        while (high < size)
            element[low++] = element[high++];
        size = low;
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
        T e = element[r];
        remove(r, r + 1);
        return e;
    }
    template <class Opt>
    void traverse(Opt &opt)
    {
        for (Rank i = 0; i < size; i++)
            opt(element[i]);
    }
    // 对区间[low, high)进行二分查找
    Rank binSearch(const T &e, Rank low, Rank high)
    {
        while (low < high)
        {
            Rank middle = (low + high) / 2;
            (e < element[middle]) ? high = middle : low = middle + 1;
        }
        return low - 1;
    };
    // 对区间[low, high)进行斐波那契查找
    Rank fibSearch(const T &e, Rank low, Rank high)
    {
        for (Fibonacci fib(high - low); low < high;)
        {
            while (high - low < fib.get())
                fib.prev();                    // fib(k)永远在[low, high)区间内
            Rank middle = low + fib.get() - 1; // 轴点为fib(k-1)-1
            e < element[middle] ? high = middle : low = middle + 1;
        }
        return --low; // 循环结束时，low为大于e的最小秩，所以要--
    }
    // 对区间[low, high)进行冒泡排序，逆序对改进版
    void bubbleSort(Rank low, Rank high)
    {
        for (Rank last = low; low < high; high = last)
            for (Rank i = (last = low) + 1; i < high; i++)
                if (element[i - 1] > element[i])
                    swap(element[i - 1], element[last = i]);
    }
    // 对全向量进行冒泡排序
    void bubbleSort() { bubbleSort(0, size); }
    // 对区间[low, high)进行归并排序
    void mergeSort(Rank low, Rank high)
    {
        if (high - low < 2)
            return;
        Rank middle = (low + high) / 2;
        mergeSort(low, middle);
        mergeSort(middle, high);
        merge(low, middle, high);
    }
    // 对全向量进行归并排序
    void mergeSort() { mergeSort(0, size); }
    friend Rank match(Vector<T> Pattern, Vector<T> Text)
    {
        Rank n = Text.size, m = Pattern.size, i = 0, j = 0;
        while (i < n && j < m)
            if (Text[i] == Pattern[j])
                i++, j++;
            else
                i -= j - 1, j = 0;
        return i - j;
    }

protected:
    Rank capacity;
    Rank size;
    T *element;

private:
    void copyFrom(const T *element, Rank low, Rank high)
    {
        this->element = new T[capacity = std::max(DEFAULT_CAPACITY, 2 * (high - low))];
        for (size = 0; low < high; size++, low++)
            this->element[size] = element[low]; // 不能使用malloc，因为malloc只能实现浅拷贝
    }
    // 空间扩容
    void expand()
    {
        if (size < capacity)
            return;
        if (capacity < DEFAULT_CAPACITY)
            capacity = DEFAULT_CAPACITY;
        T *oldElement = element;
        element = new T[capacity <<= 1];
        for (Rank i = 0; i < size; i++)
            element[i] = oldElement[i];
        delete[] oldElement;
    }
    // 空间缩容
    void shrink()
    {
        if (capacity < DEFAULT_CAPACITY << 1 || size << 2 > capacity)
            return;
        T *oldElement = element;
        element = new T[capacity >>= 1];
        for (Rank i = 0; i < size; i++)
            element[i] = oldElement[i];
        delete[] oldElement;
    }
    // mergeSort中的合并操作
    void merge(Rank low, Rank middle, Rank high)
    {
        Rank a = 0, b = 0, c = 0;
        T *A = element + low; // [low, high)
        Rank BLength = middle - low;
        T *B = new T[BLength]; // [low, middle)
        for (Rank i = 0; i < BLength; i++)
            B[i] = A[i];
        Rank CLength = high - middle;
        T *C = element + middle; // [middle, high)
        while ((b < BLength) && (c < CLength))
            A[a++] = (B[b] < C[c]) ? B[b++] : C[c++];
        while (b < BLength)
            A[a++] = B[b++];
        delete[] B;
    }
};

#endif