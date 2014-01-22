#pragma once
#include <stddef.h>
#include <stdint.h>
#include "_syscall.h"

#define TLS   (*_TLSPtr)
#define MBOX  (&(TLS->box))

typedef struct
{
    struct
    {
        uint32_t n;
    } tag;
    uint32_t reg[64];
} _MsgBox;

typedef struct
{
    _MsgBox box;
    uint8_t mem[0x1000 - sizeof(_MsgBox)];
} _TLS;

extern _TLS* volatile* const _TLSPtr;

_SYSCALL_1(0, put, char);
_SYSCALL_2(1, send_receive, uint16_t, uint16_t);
_SYSCALL_1(2, _sbrk, size_t);
