#include <assert.h>     // assert.
#include <stdio.h>      // printf.
#include "arch.h"       // arch_init.
#include "multiboot.h"  // multiboot_info_t, MULTIBOOT_BOOTLOADER_MAGIC.
#include "pmem.h"       // pmem_init.
#include "tty.h"        // tty_init, STEP.

void main(unsigned long magic, multiboot_info_t* info)
{
    tty_init();

    assert(magic == MULTIBOOT_BOOTLOADER_MAGIC);

    printf("\t\t\t\t\t\t\t\t   \eF12[ Utopia ]\n\n");
    printf("\eF09Initializing the microkernel:\n");

    arch_init();
    STEP(pmem_init(info), "Initializing Physical Memory...");
}
