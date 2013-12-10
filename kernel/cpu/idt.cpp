#include "gdt.hpp"
#include "idt.hpp"

namespace IDT
{

struct IdtEntry
{
    uint16_t offsetLow;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  flags;
    uint16_t offsetHigh;
} __attribute__((packed));

IdtEntry idt[256];

extern "C" void idt_load(IdtEntry* base, uint16_t limit);

void set_gate(uint8_t i, void* offset)
{
    idt[i].offsetLow  = (uintptr_t)offset & 0xFFFF;
    idt[i].offsetHigh = (uintptr_t)offset >> 16;
    idt[i].selector   = GDT::KERNEL_CODE;
    idt[i].zero       = 0;
    idt[i].flags      = 0x8E;
}

void init()
{
    idt_load(idt, sizeof(idt));
}

}
