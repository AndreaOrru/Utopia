#include "layout.h"
#include "scheduler.h"
#include "vmem.h"
#include "process.h"

static Process* const PCBs = (Process*)PCB_START;
static uint16_t next_pid = 1;

void* sbrk(size_t incr)
{
    Process* current = scheduler_current()->process;

    void* ret = (void*)USER_HEAP + current->heapSize;
    current->heapSize += incr;
    
    for (void* i = PAGE_BASE(ret); i < PAGE_ALIGN(ret + current->heapSize); i += PAGE_SIZE)
        map(i, NULL, PAGE_WRITE | PAGE_USER);

    return ret;
}

void process_create(ElfHeader* elf)
{
    Process* process = &PCBs[next_pid];
    map(process, NULL, PAGE_WRITE | PAGE_GLOBAL);

    process->pid = next_pid++;
    process->PD  = new_address_space();
    process->heapSize = 0;
    process->nextLocalTid = 1;
    list_init(&process->threads);

    write_cr3(process->PD);
    thread_create(elf_load(elf), process);
}
