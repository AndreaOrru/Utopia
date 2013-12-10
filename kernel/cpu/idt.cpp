#include "gdt.hpp"
#include "interrupts.hpp"
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

void set_gate(uint8_t i, void (*offset)(void))
{
    idt[i].offsetLow  = (uintptr_t)offset & 0xFFFF;
    idt[i].offsetHigh = (uintptr_t)offset >> 16;
    idt[i].selector   = GDT::KERNEL_CODE;
    idt[i].zero       = 0;
    idt[i].flags      = 0x8E;
}

void init()
{
    set_gate( 0, isr0);
    set_gate( 1, isr1);
    set_gate( 2, isr2);
    set_gate( 3, isr3);
    set_gate( 4, isr4);
    set_gate( 5, isr5);
    set_gate( 6, isr6);
    set_gate( 7, isr7);
    set_gate( 8, isr8);
    set_gate( 9, isr9);
    set_gate(10, isr10);
    set_gate(11, isr11);
    set_gate(12, isr12);
    set_gate(13, isr13);
    set_gate(14, isr14);
    set_gate(15, isr15);
    set_gate(16, isr16);
    set_gate(17, isr17);
    set_gate(18, isr18);
    set_gate(19, isr19);
    set_gate(20, isr20);
    set_gate(21, isr21);
    set_gate(22, isr22);
    set_gate(23, isr23);
    set_gate(24, isr24);
    set_gate(25, isr25);
    set_gate(26, isr26);
    set_gate(27, isr27);
    set_gate(28, isr28);
    set_gate(29, isr29);
    set_gate(30, isr30);
    set_gate(31, isr31);

    idt_load(idt, sizeof(idt));
}

}
