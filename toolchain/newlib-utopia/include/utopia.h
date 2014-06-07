#pragma once
#include <stdbool.h>
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
_SYSCALL_1(3, irq_subscribe, uint8_t);
_SYSCALL_1(4, irq_wait, uint8_t);
_SYSCALL_1(5, inb, uint16_t);
_SYSCALL_1(6, thread_create, void*);
_SYSCALL_0(7, process_exit);
_SYSCALL_4(8, _memory_map, void*, void*, size_t, bool);

void* memory_map(void* pAddr, size_t size, bool writable);
