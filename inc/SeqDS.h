#ifndef _SEQDS_H
#define _SEQDS_H

#include "Macro.h"

// 顺序存储数据结构
template <class T>
class SeqDS
{
public:
    SeqDS() : capacity(MAX_N), length(0), data(new T[capacity]){};
    SeqDS(T *data, int32_t length) : capacity(MAX_N), length(length), data(new T[capacity]) { memcpy(this->data, data, length * sizeof(T)); };
    // 初始化时深拷贝
    SeqDS(const SeqDS<T> &copied) : capacity(copied.capacity), length(copied.length), data{new T[copied.capacity]} { memcpy(this->data, copied.data, copied.length * sizeof(T)); };
    ~SeqDS() { delete[] data; }
    // 重载[]，右值版
    T &operator[](int32_t index) { return data[index]; }
    // 重载[]，左值版
    T operator[](int32_t index) const { return data[index]; }
    // 输出结构中所有元素
    friend std::ostream &operator<<(std::ostream &os, const SeqDS &u)
    {
        for (int32_t i = 0; i < u.getLength(); i++)
            os << u[i] << " ";
        return os;
    }
    int32_t getLength() const { return length; }
    bool isEmpty() const { return length == 0; }
    bool isFull() const { return length == capacity; }
    // 清空结构
    void clear() { length = 0; }
    // 判断e是否在结构中
    bool isInside(const T &e) const
    {
        for (int32_t i = 0; i < length; i++)
            if (e == data[i])
                return true;
        return false;
    }

protected:
    const int32_t capacity; // 容器容量
    int32_t length;         // 元素计数器
    int32_t *data;          // 元素
};

#endif