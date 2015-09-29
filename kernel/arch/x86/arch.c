#include <gdt.h>   // gdt_init.
#include <idt.h>   // idt_init.
#include "tty.h"   // STEP.
#include <arch.h>

void arch_init(void)
{
    STEP(gdt_init(), "Initializing the GDT...");
    STEP(idt_init(), "Initializing the IDT...");
}
