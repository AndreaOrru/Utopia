#pragma once
#include <stdint.h>

#define EVERYONE  ((uint16_t) -1)

typedef struct
{
    struct
    {
        uint32_t n;
    } tag;
    uint32_t regs[64];
} VRegs;

void send_receive(uint16_t to, uint16_t from);
