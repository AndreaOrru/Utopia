#include "arch/x86/gdt.h"
#include "arch/x86/idt.h"
#include "arch.h"

void arch_init(void)
{
    gdt_init();
    idt_init();
}
