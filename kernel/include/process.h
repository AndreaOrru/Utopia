#pragma once
#include <stdint.h>  // uint*_t.
#include <elf.h>     // ELFHeader.
#include "list.h"    // List.

typedef struct
{
    uint16_t pid;
    void* addrspace;

    uint8_t next_local_tid;
    List threads;
} Process;

void process_create(ELFHeader* elf);
void process_exit(void);
