#pragma once

/**
 * @brief 词条模板类
 * @tparam Key 关键码
 * @tparam Value 数值
 */
template<typename Key, typename Value>
struct Entry
{
    Key   key;   // 关键码
    Value value; // 数值
    explicit Entry(Key key = Key(), Value value = Value()) : key(key), value(value) {};
    Entry(const Entry<Key, Value>& e) : key(e.key), value(e.value) {};
    bool operator<(const Entry<Key, Value>& e) { return key < e.key; }
    bool operator>(const Entry<Key, Value>& e) { return key > e.key; }
    bool operator<=(const Entry<Key, Value>& e) { return key <= e.key; }
    bool operator>=(const Entry<Key, Value>& e) { return key >= e.key; }
    bool operator==(const Entry<Key, Value>& e) { return key == e.key; }
    bool operator!=(const Entry<Key, Value>& e) { return key != e.key; }
};