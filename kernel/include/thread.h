#pragma once
#include <arch.h>       // PAGE_SIZE.
#include <interrupt.h>  // Context.
#include <stdint.h>     // uint16_t.
#include "list.h"       // Link.
#include "process.h"    // Process.

typedef struct
{
    Context context;
    Process* process;

    uint16_t tid;
    uint8_t local_tid;

    Link queue_link;
    Link process_link;
} Thread;

void thread_create(const void* entry);
void thread_exit(Thread* thread);
