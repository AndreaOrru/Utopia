#include <arch.h>       // arch_init.
#include <assert.h>     // assert.
#include <stdio.h>      // printf.
#include <vmem.h>       // vmem_init.
#include "multiboot.h"  // multiboot_info_t, MULTIBOOT_BOOTLOADER_MAGIC.
#include "pmem.h"       // pmem_init.
#include "scheduler.h"  // scheduler_init.
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
}
