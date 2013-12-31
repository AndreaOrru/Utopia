#pragma once
#include <stdint.h>

typedef struct
{
    struct
    {
        uint32_t n;
    } tag;
    uint32_t reg[64];
} MsgBox;

void send_receive(uint16_t to, uint16_t from);
