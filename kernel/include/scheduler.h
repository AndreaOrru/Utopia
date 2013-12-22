#pragma once
#include "isr.h"
#include "elf.h"
#include "list.h"
#include "x86.h"

typedef struct
{
    uint16_t pid;
    void* PD;

    List threads;
} Process;

typedef union
{
    struct
    {
        uint16_t tid;
        Link queueLink;

        Process* process;
        Link processLink;
    };
    struct
    {
        uint8_t kernelStack[PAGE_SIZE - sizeof(State)];
        State state;
    };
} Thread;

void scheduler_init(void);
void create_process(ElfHeader* elf);
void create_thread(const void* entry);
