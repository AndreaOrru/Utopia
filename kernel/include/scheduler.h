#pragma once
#include "isr.h"
#include "list.h"
#include "x86.h"

typedef union
{
    struct
    {
        uint16_t tid;
        Link queueLink;
    };
    struct
    {
        uint8_t kernelStack[PAGE_SIZE - sizeof(State)];
        State state;
    };
} Thread;

void scheduler_init(void);
void create_thread(void* entry);
