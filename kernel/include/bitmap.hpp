#pragma once
#include "string.h"
#include "x86.hpp"

template <size_t bits>
class Bitmap
{
  private:
    static const size_t words = (bits + 31) / 32;
    uint32_t bitmap[words];

  public:
    Bitmap() { memset(bitmap, 0, words / 4); }

    inline bool test (uint32_t b) { return (bitmap[b / 32] >> (b % 32)) & 1; }
    inline void set  (uint32_t b) { bitmap[b / 32] |=   (1 << (b % 32)); }
    inline void reset(uint32_t b) { bitmap[b / 32] &=  ~(1 << (b % 32)); }

    uint32_t find_zero()
    {
        for (uint32_t i = 0; i < words; i++)
            if (bitmap[i] != 0xFFFFFFFF)
                return (i * 32) + ffz(bitmap[i]);
        return -1;
    }
};
