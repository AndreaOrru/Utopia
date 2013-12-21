#include "exception.h"
#include "gdt.h"
#include "irq.h"
#include "x86.h"
#include "idt.h"

typedef struct
{
    uint16_t offsetLow;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  flags;
    uint16_t offsetHigh;
} __attribute__((packed)) IdtEntry;

static IdtEntry idt[256];

void idt_set_gate(uint8_t i, IsrStub offset)
{
    idt[i].offsetLow  = (uintptr_t)offset  & 0xFFFF;
    idt[i].offsetHigh = (uintptr_t)offset >> 16;
    idt[i].selector   = KERNEL_CODE;
    idt[i].zero       = 0;
    idt[i].flags      = 0x8E;
}

void idt_init(void)
{
    exception_init();
    irq_init();

    idt_load((uintptr_t)idt, sizeof(idt));
}
