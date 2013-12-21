#pragma once
#include "isr.h"
#include "elf.h"
#include "list.h"
#include "x86.h"

typedef union
{
    struct
    {
        uint16_t tid;
        Link queueLink;
        Link childLink;

        union Thread* parent;
        List childs;
        void* PD;
    };
    struct
    {
        uint8_t kernelStack[PAGE_SIZE - sizeof(State)];
        State state;
    };
} Thread;

void scheduler_init(void);
void create_process(ElfHeader* elf);
Thread* create_thread(const void* entry);
