#include "exceptions.hpp"
#include "gdt.hpp"
#include "irqs.hpp"
#include "machine.hpp"
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

void set_gate(uint8_t i, IsrStub offset)
{
    idt[i].offsetLow  = (uintptr_t)offset  & 0xFFFF;
    idt[i].offsetHigh = (uintptr_t)offset >> 16;
    idt[i].selector   = GDT::KERNEL_CODE;
    idt[i].zero       = 0;
    idt[i].flags      = 0x8E;
}

void init()
{
    Exceptions::init();
    IRQs::init();

    idt_load((uintptr_t)idt, sizeof(idt));
}

}
