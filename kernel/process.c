#include <assert.h>   // assert.
#include <layout.h>   // PCB_START.
#include <stddef.h>   // NULL.
#include <vmem.h>     // map, addrspace_*, PAGE_*.
#include "thread.h"   // thread_create.
#include "process.h"

extern Process* current_process;

static Process* const PCBs = (Process*) PCB_START;
static uint16_t next_pid = 1;

void process_create(ELFHeader* elf)
{
    assert(elf != NULL);

    Process* process = &PCBs[next_pid];
    map(process, NULL, PAGE_WRITE | PAGE_GLOBAL);

    process->pid = next_pid++;
    process->addrspace = addrspace_create();
    process->next_local_tid = 1;
    list_init(&process->threads);

    current_process = process;
    addrspace_switch(process->addrspace);

    thread_create(elf_load(elf));
}

void process_exit(void)
{
    while (!list_empty(&current_process->threads))
        thread_exit(list_item(list_pop(&current_process->threads), Thread, process_link));
}
