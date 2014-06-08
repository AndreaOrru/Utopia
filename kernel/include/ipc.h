#pragma once
#include <stdint.h>

typedef struct
{
    struct
    {
        uint16_t pid;
        uint16_t tid;
        uint32_t n;
    } tag;
    uint32_t reg[64];
} __attribute__((packed)) MsgBox;

void send_receive(uint16_t to, uint16_t from);
