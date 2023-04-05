#pragma once

#include "Fibonacci.h"
#include "Macro.h"
#include <cstdlib>
#include <type_traits>


#define use_quickSort_Iterative true
#define use_partition_LGU true

template<typename T>
class Vector
{
public:
    explicit Vector(Rank capacity = DEFAULT_CAPACITY, Rank size = 0) :
        m_capacity(capacity), m_size(size), m_element(new T[capacity]) {};
    Vector(Rank capacity, Rank size, T element) : m_capacity(capacity), m_size(size), m_element(new T[capacity])
    {
        for (Rank i = 0; i < size; i++)
            this->m_element[i] = element;
    };
    Vector(const T* A, Rank size) { copyFrom(A, 0, size); }
    Vector(const T* A, Rank low, Rank high) { copyFrom(A, low, high); }
    Vector(const Vector<T>& V) { copyFrom(V.m_element, 0, V.m_size); }
    Vector(const Vector<T>& V, Rank low, Rank high) { copyFrom(V.m_element, low, high); }
    ~Vector() { delete[] m_element; }
    // 重载[]，左值版
    T& operator[](Rank r) { return m_element[r]; }
    // 重载[]，右值版
    T& operator[](Rank r) const { return m_element[r]; }
    // 深复制
    Vector<T>& operator=(const Vector<T>& v);
    Rank       size() const { return m_size; }
    bool       empty() const { return !m_size; }
    bool       full() const { return m_size == m_capacity; }
    // 清空结构
    void clear() { m_size = m_capacity = 0, delete[] m_element, m_element = nullptr; }
    /**
     * @brief 插入元素
     * @param r 指定秩
     * @param e 元素
     */
    Rank insert(Rank r, const T& e);
    // 在末尾插入元素
    Rank insert(const T& e) { return insert(m_size, e); }
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
        T e = m_element[r];
        remove(r, r + 1);
        return e;
    }
    template<class VST>
    void traverse(VST& visit)
    {
        for (Rank i = 0; i < m_size; i++)
            visit(m_element[i]);
    }
    // 有序向量对区间[low, high)进行二分查找
    Rank binSearch(const T& e, Rank low, Rank high);
    // 有序向量对区间[low, high)进行斐波那契查找
    Rank fibSearch(const T& e, Rank low, Rank high);
    // 有序向量中查找e的秩，通过二分查找实现
    Rank search(const T& e) { return (m_size <= 0) ? -1 : binSearch(e, 0, m_size); }
    // 无序向量对区间[low, high)查找e的秩
    Rank find(const T& e, Rank low, Rank high) const
    {
        while ((low < high--) && e != m_element[high])
            ;
        return high;
    }
    // 无序向量中查找e的秩
    Rank find(const T& e) const { return find(e, 0, m_size); }
    // 对区间[low, high)进行冒泡排序，逆序对改进版
    void bubbleSort(Rank low, Rank high);
    // 对全向量进行冒泡排序
    void bubbleSort() { bubbleSort(0, m_size); }
    // 对区间[low, high)进行归并排序
    void mergeSort(Rank low, Rank high);
    // 对全向量进行归并排序
    void mergeSort() { mergeSort(0, m_size); }
    // 对区间[low, high)进行快速排序
    void quickSort(Rank low, Rank high);
    // 对全向量进行快速排序
    void quickSort() { quickSort(0, m_size); }

protected:
    Rank m_capacity;
    Rank m_size;
    T*   m_element;
    // 元素拷贝
    void copyFrom(const T* element, Rank low, Rank high);

private:
    // 空间扩容
    void expand();
    // 空间缩容
    void shrink();
    // mergeSort中的合并操作
    void merge(Rank low, Rank middle, Rank high);
    // 轴点构造算法：通过调整元素位置构造区间[low, high)的轴点，并返回其秩
    Rank partition(Rank low, Rank high);
};

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& v)
{
    if (this == &v)
        return *this;
    delete[] m_element;
    copyFrom(v.m_element, 0, v.m_size);
    return *this;
};

template<typename T>
Rank Vector<T>::insert(Rank r, const T& e)
{
    expand();
    for (Rank i = m_size; i > r; i--)
        m_element[i] = m_element[i - 1];
    m_element[r] = e;
    m_size++;
    return r;
}

template<typename T>
Rank Vector<T>::remove(Rank low, Rank high)
{
    if (low == high)
        return 0;
    while (high < m_size)
        m_element[low++] = m_element[high++];
    m_size = low;
    shrink();
    return high - low;
}

template<typename T>
Rank Vector<T>::binSearch(const T& e, Rank low, Rank high)
{
    while (low < high)
    {
        Rank middle = (low + high) / 2;
        (e < m_element[middle]) ? high = middle : low = middle + 1;
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
        e < m_element[middle] ? high = middle : low = middle + 1;
    }
    return --low; // 循环结束时，low为大于e的最小秩，所以要--
}

template<typename T>
void Vector<T>::bubbleSort(Rank low, Rank high)
{
    for (Rank last = low; low < high; high = last)
        for (Rank i = (last = low) + 1; i < high; i++)
            if (m_element[i - 1] > m_element[i])
                swap(m_element[i - 1], m_element[last = i]);
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

#if use_partition_LGU
template<typename T>
Rank Vector<T>::partition(Rank low, Rank high)
{ // LGU: pivot=[low], L=(low, middle], G=(middle, k), U=[k, high]
    std::swap(m_element[low], m_element[low + rand() % (high - low)]); // 任选一个元素与首元素交换
    T    pivot  = m_element[low]; // 以首元素为候选轴点——经以上交换，等效于随机选取
    Rank middle = low;
    for (Rank k = low + 1; k < high; k++)
        if (m_element[k] < pivot)
            std::swap(m_element[++middle], m_element[k]);
    std::swap(m_element[low], m_element[middle]); // 轴点归位
    return middle;
}
#else
template<typename T>
Rank Vector<T>::partition(Rank low, Rank high)
{                                                                  // DUP版本
    std::swap(element[low], element[low + rand() % (high - low)]); // 任选一个元素与首元素交换
    T pivot = element[low]; // 以首元素为候选轴点——经以上交换，等效于随机选取
    high--;
    while (low < high)
    { // 从向量两端交替向中间扫描
        while (low < high)
            if (pivot < element[high]) // 自右向左收拢
                high--;
            else // 直到遇到不大于pivot者，将其放入左侧空闲处
            {
                element[low++] = element[high];
                break;
            }
        while (low < high)
            if (element[low] < pivot) // 自左向右收拢
                low++;
            else // 直到遇到不小于pivot者，将其放入右侧空闲处
            {
                element[high--] = element[low];
                break;
            }
    }
    element[low] = pivot; // 将轴点放到该放的位置
    return low;           // 返回轴点秩
}
#endif

#if use_quickSort_Iterative
template<typename T>
void Vector<T>::quickSort(Rank low, Rank high)
{ // TODO: iterative quickSort
    if (high - low < 2)
        return;
    Rank pivot = partition(low, high);
    quickSort(low, pivot);
    quickSort(pivot + 1, high);
}
#else
template<typename T>
void Vector<T>::quickSort(Rank low, Rank high)
{
    if (high - low < 2)
        return;
    Rank pivot = partition(low, high);
    quickSort(low, pivot);
    quickSort(pivot + 1, high);
}
#endif

template<typename T>
void Vector<T>::copyFrom(const T* element, Rank low, Rank high)
{
    this->m_element = new T[m_capacity = std::max(DEFAULT_CAPACITY, 2 * (high - low))];
    for (m_size = 0; low < high; m_size++, low++)
        this->m_element[m_size] = element[low]; // 不能使用malloc，因为malloc只能实现浅拷贝
}

template<typename T>
void Vector<T>::expand()
{
    if (m_size < m_capacity)
        return;
    if (m_capacity < DEFAULT_CAPACITY)
        m_capacity = DEFAULT_CAPACITY;
    T* old_element = m_element;
    m_element      = new T[m_capacity <<= 1];
    for (Rank i = 0; i < m_size; i++)
        m_element[i] = old_element[i];
    delete[] old_element;
}

template<typename T>
void Vector<T>::shrink()
{
    if (m_capacity < DEFAULT_CAPACITY << 1 || m_size << 2 > m_capacity)
        return;
    T* old_element = m_element;
    m_element      = new T[m_capacity >>= 1];
    for (Rank i = 0; i < m_size; i++)
        m_element[i] = old_element[i];
    delete[] old_element;
}

template<typename T>
void Vector<T>::merge(Rank low, Rank middle, Rank high)
{
    Rank a = 0, b = 0, c = 0;
    T*   a_array  = m_element + low; // [low, high)
    Rank b_length = middle - low;
    T*   b_array  = new T[b_length]; // [low, middle)
    for (Rank i = 0; i < b_length; i++)
        b_array[i] = a_array[i];
    Rank c_length = high - middle;
    T*   c_array  = m_element + middle; // [middle, high)
    while ((b < b_length) && (c < c_length))
        a_array[a++] = (b_array[b] < c_array[c]) ? b_array[b++] : c_array[c++];
    while (b < b_length)
        a_array[a++] = b_array[b++];
    delete[] b_array;
}

#undef use_quickSort_Iterative
#undef use_partition_LGU