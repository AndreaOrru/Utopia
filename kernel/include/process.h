#pragma once
#include "elf.h"
#include "list.h"

typedef struct
{
    uint16_t pid;
    void* PD;

    uint16_t nextLocalTid;
    List threads;
} Process;

void process_create(ElfHeader* elf);
