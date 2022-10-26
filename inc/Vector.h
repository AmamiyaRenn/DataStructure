#ifndef _VECTOR_H
#define _VECTOR_H

#include "Fibonacci.h"
#include "Macro.h"

template<typename T>
class Vector
{
public:
    explicit Vector(Rank capacity = DEFAULT_CAPACITY, Rank size = 0) :
        capacity(capacity), size(size), element(new T[capacity]) {};
    Vector(Rank capacity, Rank size, T element) : capacity(capacity), size(size), element(new T[capacity])
    {
        for (Rank i = 0; i < size; i++)
            this->element[i] = element;
    };
    Vector(const T* A, Rank size) { copyFrom(A, 0, size); }
    Vector(const T* A, Rank low, Rank high) { copyFrom(A, low, high); }
    Vector(const Vector<T>& V) { copyFrom(V.element, 0, V.size); }
    Vector(const Vector<T>& V, Rank low, Rank high) { copyFrom(V.element, low, high); }
    ~Vector() { delete[] element; }
    // 重载[]，左值版
    T& operator[](Rank r) { return element[r]; }
    // 重载[]，右值版
    T& operator[](Rank r) const { return element[r]; }
    // 深复制
    Vector<T>& operator=(const Vector<T>& v);
    Rank       getSize() const { return size; }
    bool       empty() const { return !size; }
    bool       full() const { return size == capacity; }
    // 清空结构
    void clear() { size = capacity = 0, delete[] element, element = nullptr; }
    /**
     * @brief 插入元素
     * @param r 指定秩
     * @param e 元素
     */
    Rank insert(Rank r, const T& e);
    // 在末尾插入元素
    Rank insert(const T& e) { return insert(size, e); }
    /**
     * @brief 删除[low, high)的元素
     * @param low 左闭
     * @param high 右开
     * @return Rank 被删除的元素个数
     */
    Rank remove(Rank low, Rank high);
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
    template<class VST>
    void traverse(VST& visit)
    {
        for (Rank i = 0; i < size; i++)
            visit(element[i]);
    }
    // 对区间[low, high)进行二分查找
    Rank binSearch(const T& e, Rank low, Rank high);
    // 对区间[low, high)进行斐波那契查找
    Rank fibSearch(const T& e, Rank low, Rank high);
    // 有序向量中查找e的秩，通过二分查找实现
    Rank search(const T& e) const { return (size <= 0) ? -1 : binSearch(e, 0, size); }
    // 对区间[low, high)进行冒泡排序，逆序对改进版
    void bubbleSort(Rank low, Rank high);
    // 对全向量进行冒泡排序
    void bubbleSort() { bubbleSort(0, size); }
    // 对区间[low, high)进行归并排序
    void mergeSort(Rank low, Rank high);
    // 对全向量进行归并排序
    void        mergeSort() { mergeSort(0, size); }
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
    T*   element;

private:
    // 元素拷贝
    void copyFrom(const T* element, Rank low, Rank high);
    // 空间扩容
    void expand();
    // 空间缩容
    void shrink();
    // mergeSort中的合并操作
    void merge(Rank low, Rank middle, Rank high);
};

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& v)
{
    if (this == &v)
        return *this;
    delete[] element;
    copyFrom(v.element, 0, v.size);
    return *this;
};

template<typename T>
Rank Vector<T>::insert(Rank r, const T& e)
{
    expand();
    for (Rank i = size; i > r; i--)
        element[i] = element[i - 1];
    element[r] = e;
    size++;
    return r;
}

template<typename T>
Rank Vector<T>::remove(Rank low, Rank high)
{
    if (low == high)
        return 0;
    while (high < size)
        element[low++] = element[high++];
    size = low;
    shrink();
    return high - low;
}

template<typename T>
Rank Vector<T>::binSearch(const T& e, Rank low, Rank high)
{
    while (low < high)
    {
        Rank middle = (low + high) / 2;
        (e < element[middle]) ? high = middle : low = middle + 1;
    }
    return low - 1;
};

template<typename T>
Rank Vector<T>::fibSearch(const T& e, Rank low, Rank high)
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

template<typename T>
void Vector<T>::bubbleSort(Rank low, Rank high)
{
    for (Rank last = low; low < high; high = last)
        for (Rank i = (last = low) + 1; i < high; i++)
            if (element[i - 1] > element[i])
                swap(element[i - 1], element[last = i]);
}

template<typename T>
void Vector<T>::mergeSort(Rank low, Rank high)
{
    if (high - low < 2)
        return;
    Rank middle = (low + high) / 2;
    mergeSort(low, middle);
    mergeSort(middle, high);
    merge(low, middle, high);
}

template<typename T>
void Vector<T>::copyFrom(const T* element, Rank low, Rank high)
{
    this->element = new T[capacity = std::max(DEFAULT_CAPACITY, 2 * (high - low))];
    for (size = 0; low < high; size++, low++)
        this->element[size] = element[low]; // 不能使用malloc，因为malloc只能实现浅拷贝
}

template<typename T>
void Vector<T>::expand()
{
    if (size < capacity)
        return;
    if (capacity < DEFAULT_CAPACITY)
        capacity = DEFAULT_CAPACITY;
    T* old_element = element;
    element        = new T[capacity <<= 1];
    for (Rank i = 0; i < size; i++)
        element[i] = old_element[i];
    delete[] old_element;
}

template<typename T>
void Vector<T>::shrink()
{
    if (capacity < DEFAULT_CAPACITY << 1 || size << 2 > capacity)
        return;
    T* old_element = element;
    element        = new T[capacity >>= 1];
    for (Rank i = 0; i < size; i++)
        element[i] = old_element[i];
    delete[] old_element;
}

template<typename T>
void Vector<T>::merge(Rank low, Rank middle, Rank high)
{
    Rank a = 0, b = 0, c = 0;
    T*   a_array  = element + low; // [low, high)
    Rank b_length = middle - low;
    T*   b_array  = new T[b_length]; // [low, middle)
    for (Rank i = 0; i < b_length; i++)
        b_array[i] = a_array[i];
    Rank c_length = high - middle;
    T*   c_array  = element + middle; // [middle, high)
    while ((b < b_length) && (c < c_length))
        a_array[a++] = (b_array[b] < c_array[c]) ? b_array[b++] : c_array[c++];
    while (b < b_length)
        a_array[a++] = b_array[b++];
    delete[] b_array;
}

#endif