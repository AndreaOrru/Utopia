#include <arch.h>       // arch_init.
#include <asm.h>        // sti.
#include <assert.h>     // assert.
#include <stdbool.h>    // bool.
#include <stdio.h>      // printf.
#include <vmem.h>       // vmem_init.
#include "multiboot.h"  // multiboot_info_t, MULTIBOOT_BOOTLOADER_MAGIC.
#include "pmem.h"       // pmem_init.
#include "process.h"    // process_create.
#include "scheduler.h"  // scheduler_init.
#include "syscall.h"    // syscall_init.
#include "timer.h"      // timer_init.
#include "tty.h"        // tty_init, STEP.

void main(unsigned long magic, const multiboot_info_t* const info)
{
    tty_init();

    assert(magic == MULTIBOOT_BOOTLOADER_MAGIC);

    printf("\t\t\t\t\t\t\t\t   \eF12[ Utopia ]\n\n");
    printf("\eF09Initializing the kernel:\n");

    arch_init();
    STEP(pmem_init(info),  "Initializing Physical Memory...");
    STEP(vmem_init(),      "Initializing Virtual Memory...");
    STEP(timer_init(50),   "Initializing the system timer...");
    STEP(scheduler_init(), "Initializing the scheduler...");
    STEP(syscall_init(),   "Initializing syscalls...");

    multiboot_module_t* mods = (void*) info->mods_addr;
    for (unsigned i = 0; i < info->mods_count; i++)
        process_create((ELFHeader*) mods[i].mod_start);

    sti();
    while (true);
}
