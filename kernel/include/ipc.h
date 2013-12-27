#pragma once
#include <stdint.h>

typedef struct
{
    uint32_t tag;
    uint32_t regs[64];
} UTCB;
