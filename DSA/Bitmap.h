#pragma once

#include "DSA/Macro.h"

class Bitmap
{
public:
    explicit Bitmap(Rank capacity = 8) { init(capacity); };
    ~Bitmap() { delete[] map; }
    void set(Rank k) { map[k >> 3] |= (0x80 >> (k & 0x7)); }
    void clear(Rank k) { map[k >> 3] &= ~(0x80 >> (k & 0x7)); }
    bool test(Rank k) { return map[k >> 3] &= (0x80 >> (k & 0x7)); }

protected:
    void init(Rank capacity) { map = new uint8_t[bytenum = (capacity + 7) / 8]; }

private:
    uint8_t* map;
    Rank     bytenum;
};