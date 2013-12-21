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
    timer_init(100);
    scheduler_init();

    printf("Hello world!\n");

    while (true);
}
