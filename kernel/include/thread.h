#pragma once
#include "interrupt.h"
#include "process.h"
#include "x86.h"

typedef enum { NEW, READY } State;

typedef union
{
    struct
    {
        uint16_t tid;
        State state;
        Link queueLink;

        uint16_t localTid;
        Process* process;
        Link processLink;
    };
    struct
    {
        uint8_t kernelStack[PAGE_SIZE - sizeof(Context)];
        Context context;
    };
} Thread;

void thread_create(const void* entry);
