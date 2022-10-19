#pragma once

#include <stdlib.h>

// 用于机器人代码
// 循环队列类
template<typename T>
class QueueCircular
{
public:
    // 构造函数
    explicit QueueCircular(size_t capacity) :
        capacity(capacity), head_index(0), tail_index(0), data_counter(0), data(new T[capacity]) {};
    // 拷贝构造函数，实现初始化时深拷贝
    QueueCircular(const QueueCircular<T>& queueCopied) :
        capacity(queueCopied.capacity), head_index(queueCopied.head_index), tail_index(queueCopied.tail_index),
        data_counter(queueCopied.data_counter), data(new T[capacity])
    {
        for (size_t i = 0; i < data_counter; i++)
            data[(head_index + i) % capacity] = queueCopied.data[(head_index + i) % capacity];
    }
    // 析构函数
    ~QueueCircular() { delete[] data; }
    // 拷贝赋值函数，实现赋值时深拷贝
    QueueCircular& operator=(const QueueCircular& queueCopied)
    {
        if (this == &queueCopied) // 检查是否是自己
            return *this;
        delete[] data;
        capacity     = queueCopied.capacity;
        head_index   = queueCopied.head_index;
        tail_index   = queueCopied.tail_index;
        data_counter = queueCopied.data_counter;
        data         = new T[capacity];
        for (size_t i = 0; i < data_counter; i++)
            data[i] = queueCopied.data[i];
        return *this;
    }
    // 队空
    bool empty() const { return data_counter <= 0; }
    // 队满
    bool full() const { return data_counter >= capacity; }
    // 获得队首值
    T& front() const { return data[head_index]; }
    /**
     * @brief 入队
     * @param value 入队的值
     * @return true 入队成功
     * @return false 入队失败
     */
    bool enqueue(const T& value)
    {
        if (full())
            return false;
        else
        {
            data[tail_index] = value;
            tail_index       = (tail_index + 1) % capacity;
            data_counter++;
        }
        return true;
    }
    /**
     * @brief 批量入队
     * @param valueAddress 被拷贝的值的首地址
     * @param len 长度
     * @return true 入队成功
     * @return false 入队失败
     */
    bool enqueue(T* valueAddress, size_t len)
    {
        for (size_t i = 0; i < len; i++)
            if (!enqueue(*(valueAddress + i)))
                return false;
        return true;
    }
    /**
     * @brief 出队并记录出队值
     * @param value 记录出队值的值
     * @return true 出队成功
     * @return false 出队失败
     */
    bool dequeue(T& value)
    {
        if (empty())
            return false;
        else
        {
            value      = data[head_index];
            head_index = (head_index + 1) % capacity;
            data_counter--;
        }
        return true;
    }
    /**
     * @brief 出队并记录出队值
     * @param valueAddress 被拷贝的值的首地址
     * @param len 长度
     * @return true 出队成功
     * @return false 出队失败
     */
    bool dequeue(T* valueAddress, size_t len)
    {
        for (size_t i = 0; i < len; i++)
            if (!dequeue(*(valueAddress + i)))
                return false;
        return true;
    }

private:
    size_t capacity;     // 数据容量
    size_t head_index;   // 队首索引
    size_t tail_index;   // 队尾索引
    size_t data_counter; // 数据计数器
    T*     data;         // 数据，动态申请
};