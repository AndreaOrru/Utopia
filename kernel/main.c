#include "gdt.h"
#include "idt.h"
#include "pmem.h"
#include "process.h"
#include "scheduler.h"
#include "syscall.h"
#include "term.h"
#include "timer.h"
#include "vmem.h"

void main(multiboot_info_t* info)
{
    clear_screen();

    printf("\t\t\t\t\t\t\t\t   \eF12[ Utopia ]\n");
    printf("\n\eF09Initializing the microkernel:\n");

    STEP(gdt_init(),       "Initializing the GDT...");
    STEP(idt_init(),       "Initializing the IDT...");
    STEP(pmem_init(info),  "Initializing Physical Memory...");
    STEP(vmem_init(),      "Initializing Virtual Memory...");
    STEP(timer_init(50),   "Initializing the timer...");
    STEP(scheduler_init(), "Initializing the scheduler...");
    STEP(syscall_init(),   "Initializing the syscalls...");

    multiboot_module_t* mods = (void*)info->mods_addr;
    for (unsigned i = 0; i < info->mods_count; i++)
        process_create((ElfHeader*)mods[i].mod_start);

    sti();
    while (true);
}
