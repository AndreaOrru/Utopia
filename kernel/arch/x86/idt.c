#include "arch/x86/gdt.h"
#include "arch/x86/interrupt.h"
#include "arch/x86/idt.h"

typedef struct
{
    uint16_t limit;
    void* base;
} __attribute__((packed)) IDTR;

typedef struct
{
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  flags;
    uint16_t offset_high;
} __attribute__((packed)) IDTEntry;

static IDTEntry idt[256];
static IDTR idtr = { sizeof(idt), idt };

extern void idt_load(IDTR* idtr);

void idt_gate_set(uint8_t i, uint8_t flags, ISRStub offset)
{
    idt[i].offset_low  = (uintptr_t) offset  & 0xFFFF;
    idt[i].offset_high = (uintptr_t) offset >> 16;
    idt[i].selector    = KERNEL_CODE;
    idt[i].zero        = 0;
    idt[i].flags       = flags;
}

void idt_init(void)
{
    interrupt_init();

    idt_load(&idtr);
}
