#ifndef _SET_H
#define _SET_H

#include <iostream>
#include <cstdio>
#include "Macro.h"

template <class T>
class Set
{
public:
    Set() : capacity(MAX_N), length(0), data(new T[capacity]){};
    Set(T *data, int32_t length) : capacity(MAX_N), length(length), data(new T[capacity]) { memcpy(this->data, data, length * sizeof(T)); };
    // 初始化时深拷贝
    Set(const Set<T> &copied) : capacity(copied.capacity), length(copied.length), data{new T[copied.capacity]} { memcpy(this->data, copied.data, copied.length * sizeof(T)); };
    ~Set() { delete[] data; }
    // 重载[]，右值版
    T &operator[](int32_t index) { return data[index]; }
    // 重载[]，左值版
    T operator[](int32_t index) const { return data[index]; }
    // 输出集合中所有元素
    friend std::ostream &operator<<(std::ostream &os, const Set &u)
    {
        for (int32_t i = 0; i < u.getLength(); i++)
            os << u[i] << " ";
        return os;
    }
    // 返回 this_set ∪ v_set
    friend Set<T> operator+(const Set &u, const Set &v)
    {
        Set<T> result = u;
        for (int32_t i = 0; i < v.getLength(); i++)
            result.append(v[i]);
        return result;
    }
    // 返回 this_set - v_set
    friend Set<T> operator-(const Set &u, const Set &v)
    {
        Set<T> result = u;
        for (int32_t i = 0; i < v.getLength(); i++)
            result.remove(v[i]);
        return result;
    }
    // 返回 this_set ∩ v_set
    Set<T> intersect(const Set &v)
    {
        Set<T> result;
        for (int32_t i = 0; i < v.getLength(); i++)
            if (isInSet(v[i]))
                result.append(v[i]);
        return result;
    }
    int32_t getLength() const { return length; }
    // 判断e是否在集合中
    bool isInSet(const T &e) const
    {
        for (int32_t i = 0; i < length; i++)
            if (e == data[i])
                return true;
        return false;
    }
    /**
     * @brief 向集合中添加e
     * @param e 被添加的元素
     * @return true 添加成功
     * @return false 添加失败
     */
    bool append(const T &e)
    {
        if (isInSet(e))
            return false;
        data[length++] = e;
        return true;
    }
    /**
     * @brief 向集合中删除e
     * @param e 被删除的元素
     * @return true 删除成功
     * @return false 删除失败
     */
    bool remove(const T &e)
    {
        for (int32_t i = 0; i < length; i++)
            if (e == data[i])
            {
                data[i] = data[--length];
                data[length] = 0;
                return true;
            }
        return false;
    }

private:
    int32_t capacity; // 容器容量
    int32_t length;   // 元素计数器
    int32_t *data;    // 元素
};

#endif