#include "gdt.h"
#include "idt.h"
#include "pmem.h"
#include "scheduler.h"
#include "term.h"
#include "timer.h"
#include "vmem.h"

void main(multiboot_info_t* info)
{
    clear_screen();

    gdt_init();
    idt_init();
    pmem_init(info);
    vmem_init();
    timer_init(50);
    scheduler_init();

    printf("Hello world!\n");

    multiboot_module_t* mods = (void*)info->mods_addr;
    for (unsigned i = 0; i < info->mods_count; i++)
        create_process((ElfHeader*)mods[i].mod_start);

    sti();
    while (true);
}
