#ifndef _SEQLIST_H
#define _SEQLIST_H

#include "SeqDS.h"

template <class T>
class SeqList : public SeqDS<T>
{
public:
    // 一次性把所有的构造函数都引用过来
    using SeqDS::SeqDS;
    /**
     * @brief 查找对应元素第一次出现在List中的位置
     * @param e 被查找的元素
     * @return int32_t 位置(-1则为没找到)
     */
    int32_t locate(const T &e) const
    {
        for (int32_t i = 0; i < this->length; i++)
            if (e == this->data[i])
                return i;
        return -1;
    }
    /**
     * @brief 在指定位置插入元素
     * @param index 指定位置
     * @param e 被插入的元素
     * @return true 插入成功
     * @return false 插入失败
     */
    bool insert(int32_t index, const T &e)
    {
        if (index < 0 || index > this->length || this->isFull())
            return false;
        for (int32_t i = this->length; i > index; i--)
            this->data[i] = this->data[i - 1]; // TODO:用memcpy是否会更快一些？
        this->data[index] = e;
        this->length++;
        return true;
    }
    /**
     * @brief 删除指定位置的元素
     * @param index 位置
     * @return true 删除成功
     * @return false 删除失败
     */
    bool remove(int32_t index)
    {
        if (index < 0 || index >= this->length)
            return false;
        for (int i = index; i < this->length - 1; i++)
            this->data[i] = this->data[i + 1];
        this->length--;
        return true;
    }
    // 把所有负数放到非负数前
    void move_all_neg_aheadOf_nonneg()
    {
        int32_t i = 0, j = this->length - 1;
        while (true)
        {
            while (this->data[i] < 0)
                i++;
            while (this->data[j] >= 0)
                j--;
            if (i < j)
                swap(this->data[i], this->data[j]);
            else
                break;
        }
    }
    // 删除所有的最大元素
    void remove_all_maximum()
    {
        int32_t posRecorder[MAX_N] = {}, max = this->data[0], cnt = 1;
        for (int32_t i = 1; i < this->length; i++)
            if (max < this->data[i])
                max = this->data[i], posRecorder[0] = i, cnt = 1;
            else if (max == this->data[i])
                posRecorder[cnt++] = i;
        int32_t j = 0, k = 0, newData[MAX_N] = {};
        for (int32_t i = 0; i < this->length; i++)
            if (this->data[posRecorder[j]] == this->data[i] && j < cnt)
                j++;
            else
                newData[k++] = data[i];
        memcpy(this->data, newData, sizeof(T) * k);
        this->length = k;
    }
};

#endif