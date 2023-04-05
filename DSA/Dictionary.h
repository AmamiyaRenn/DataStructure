#pragma once

#include "Entry.h"
#include "Macro.h"

template<typename Key, typename Value>
class Dictionary
{
public:
    // 当前词条数
    virtual Rank size() const = 0;
    // 插入词条（key重复会无效）
    virtual bool put(Key key, Value value) = 0;
    // 读取词条
    virtual Value* get(Key key) = 0;
    // 删除词条
    virtual bool remove(Key key) = 0;
    // 返回一个空词条
    Entry<Key, Value> nullEntry() const { return null_entry; }

private:
    Entry<Key, Value> null_entry;
};
