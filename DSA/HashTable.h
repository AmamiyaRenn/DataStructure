#pragma once

#include "DSA/Bitmap.h"
#include "DSA/Dictionary.h"
#include "DSA/Macro.h"
#include "DSA/Prime.h"
#include <string.h>

template<typename Key, typename Value>
class HashTable : public Dictionary<Key, Value>
{
public:
    explicit HashTable(Rank capacity)
    {
        this->capacity = prime(capacity, capacity + 1009);
        m_size         = 0;
        bucket         = new Entry<Key, Value>[capacity];
        lazytag        = 0;
        removed        = new Bitmap(capacity);
    }
    ~HashTable()
    {
        delete[] bucket;
        delete removed;
    }
    // 当前词条数
    Rank size() const override { return m_size; }
    // 插入词条（key重复会无效）
    bool put(Key key, Value value) override
    {
        if (bucket[probe4Hit(key)] != this->nullEntry())
            return false;
        Rank r    = probe4Free(key);
        bucket[r] = Entry<Key, Value>(key, value);
        m_size++;
        if (removed->test(r))
        {
            removed->clear(r);
            lazytag--;
        }
        if ((m_size + lazytag) * 2 > capacity) // 装填因子大于50%，重散列
            rehash();
        return true;
    }
    // 读取词条
    Value* get(Key key) override
    {
        Rank r = probe4Hit(key);
        return bucket[r] != this->nullEntry() ? &bucket[r].value : nullptr;
    }
    // 删除词条
    bool remove(Key key) override
    {
        Rank r = probe4Hit(key);
        if (bucket[r] == this->nullEntry())
            return false;
        m_size--;
        removed->set(r);
        lazytag++;
        if (3 * m_size < lazytag) //若懒惰删除标记过多，重散列
            rehash();
        return true;
    }

protected:
    void rehash()
    {
        Rank               old_capacity = capacity;
        Entry<Key, Value>* old_bucket   = bucket;
        capacity                        = prime(4 * m_size, 4 * m_size + 1009);
        bucket                          = new Entry<Key, Value>[capacity];
        delete removed;
        removed = new Bitmap(capacity);
        lazytag = 0;
        for (Rank i = 0; i < capacity; i++)
            if (old_bucket[i] != this->nullEntry())
                put(old_bucket[i].key, old_bucket[i].value);
        delete[] old_bucket;
    }
    size_t hashcode(char c) { return static_cast<size_t>(c) % capacity; }
    size_t hashcode(char s[])
    {
        uint8_t code = 0; // 散列码
        for (size_t n = strlen(s), i = 0; i < n; i++)
        {                                      // 字符串的多项式散列码
            code = (code << 5) | (code >> 27); // 实验得来的magic number
            code += static_cast<size_t>(s[i]);
        }
        return static_cast<size_t>(code) % capacity;
    }
    /**
     * @brief 沿关键码key的试探链，找到词条匹配的桶
     * @return Rank key所在的位置
     */
    Rank probe4Hit(const Key& key)
    {
        Rank r = hashcode(key); // 探链指针
        while ((bucket[r] != this->nullEntry() && key != bucket[r].key) || removed->test(r))
            r = (r + 1) % capacity; // 线性试探
        return r;
    }
    /**
     * @brief 沿关键码key的试探链，找到首个可用空桶
     * @return Rank key应该被安放的位置
     */
    Rank probe4Free(const Key& key)
    {
        Rank r = hashcode(key) % capacity; // 按除余法确定试探链起点
        while (bucket[r] != this->nullEntry())
            r = (r + 1) % capacity; // 线性试探，直到首个空桶（无论是否带有懒惰删除标记）
        return r;                   // 只要有空桶，线性试探迟早能找到
    }

private:
    Rank               capacity; // 桶个数
    Rank               m_size;   // 词条数
    Rank               lazytag;  // 懶惰刪除标记
    Entry<Key, Value>* bucket;   // 词条桶数组
    Bitmap*            removed;  // 懒惰删除标记
};