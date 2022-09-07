#ifndef _SET_H
#define _SET_H

#include "SeqDS.h"

template <class T>
class Set : public SeqDS<T>
{
public:
    using SeqDS::SeqDS;
    /**
     * @brief 向Set末尾添加e
     * @param e 被添加的元素
     * @return true 添加成功
     * @return false 添加失败
     */
    bool append(const T &e)
    {
        if (this->isFull() || this->isInside(e))
            return false;
        this->data[this->length++] = e;
        return true;
    }
    /**
     * @brief 向Set中删除e
     * @param e 被删除的元素
     * @return true 删除成功
     * @return false 删除失败
     */
    bool remove(const T &e)
    {
        for (int32_t i = 0; i < this->length; i++)
            if (e == this->data[i])
            {
                this->data[i] = this->data[--this->length];
                this->data[this->length] = 0;
                return true;
            }
        return false;
    }
    // 返回 this_set ∪ v_set
    friend Set<T> operator+(const Set &u, const Set &v)
    {
        Set<T> result = u;
        for (int32_t i = 0; i < v.getlength(); i++)
            result.append(v[i]);
        return result;
    }
    // 返回 this_set - v_set
    friend Set<T> operator-(const Set &u, const Set &v)
    {
        Set<T> result = u;
        for (int32_t i = 0; i < v.getlength(); i++)
            result.remove(v[i]);
        return result;
    }
    // 返回 this_set ∩ v_set
    Set<T> intersect(const Set &v)
    {
        Set<T> result;
        for (int32_t i = 0; i < v.getlength(); i++)
            if (this->isInside(v[i]))
                result.append(v[i]);
        return result;
    }
};

#endif