#pragma once
#include "elf.h"
#include "list.h"

typedef struct
{
    uint16_t pid;
    void* PD;
    size_t heapSize;

    uint16_t nextLocalTid;
    List threads;
} Process;

void process_create(ElfHeader* elf);
void* sbrk(size_t incr);
