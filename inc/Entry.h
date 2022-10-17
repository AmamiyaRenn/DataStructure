#pragma once

/**
 * @brief 词条模板类
 * @tparam K 关键码
 * @tparam V 数值
 */
template <typename KEY, typename VALUE>
struct Entry
{
    KEY key;     // 关键码
    VALUE value; // 数值
    Entry(KEY key = KEY(), VALUE value = VALUE()) : key(key), value(value){};
    Entry(const Entry<KEY, VALUE> &e) : key(e.key), value(e.value){};
    bool operator<(const Entry<KEY, VALUE> &e) { return key < e.key; }
    bool operator>(const Entry<KEY, VALUE> &e) { return key > e.key; }
    bool operator<=(const Entry<KEY, VALUE> &e) { return key <= e.key; }
    bool operator>=(const Entry<KEY, VALUE> &e) { return key >= e.key; }
    bool operator==(const Entry<KEY, VALUE> &e) { return key == e.key; }
    bool operator!=(const Entry<KEY, VALUE> &e) { return key != e.key; }
};