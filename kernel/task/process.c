#include "layout.h"
#include "thread.h"
#include "vmem.h"
#include "process.h"

static Process* const PCBs = (Process*)PCB_START;
static uint16_t next_pid = 1;

void process_create(ElfHeader* elf)
{
    Process* process = &PCBs[next_pid];
    map(process, NULL, PAGE_WRITE | PAGE_GLOBAL);

    process->pid = next_pid++;
    process->PD  = new_address_space();
    process->nextLocalTid = 1;
    list_init(&process->threads);

    write_cr3(process->PD);
    thread_create(elf_load(elf), process);
}
