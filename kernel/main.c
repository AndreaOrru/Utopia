#include <stdbool.h>
#include "gdt.h"
#include "idt.h"
#include "pmem.h"
#include "term.h"
#include "timer.h"
#include "vmem.h"
#include "x86.h"
void main(multiboot_info_t* info)
{
    clear_screen();

    gdt_init();
    idt_init();
    pmem_init(info);
    vmem_init();
    timer_init(100);

    printf("Hello world!\n");

    while (true);
}
