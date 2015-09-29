#include <gdt.h>   // gdt_init.
#include <idt.h>   // idt_init.
#include <pit.h>   // pit_init.
#include "tty.h"   // STEP.
#include <arch.h>

void arch_init(void)
{
    STEP(gdt_init(),   "Initializing the Global Descriptor Table...");
    STEP(idt_init(),   "Initializing the Interrupt Descriptor Table...");
    STEP(pit_init(50), "Initializing the Programmable Interval Timer...");
}
