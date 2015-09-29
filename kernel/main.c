#include <arch.h>       // arch_init.
#include <assert.h>     // assert.
#include <stdio.h>      // printf.
#include <vmem.h>       // vmem_init.
#include "multiboot.h"  // multiboot_info_t, MULTIBOOT_BOOTLOADER_MAGIC.
#include "pmem.h"       // pmem_init.
#include "tty.h"        // tty_init, STEP.

void main(unsigned long magic, const multiboot_info_t* const info)
{
    tty_init();

    assert(magic == MULTIBOOT_BOOTLOADER_MAGIC);

    printf("\t\t\t\t\t\t\t\t   \eF12[ Utopia ]\n\n");
    printf("\eF09Initializing the microkernel:\n");

    arch_init();
    STEP(pmem_init(info), "Initializing the Physical Memory Manager...");
    STEP(vmem_init(),     "Initializing the Virtual Memory Manager...");
}
