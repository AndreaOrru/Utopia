#include "gdt.hpp"
#include "isr.hpp"
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
    idt[i].offsetLow  = (uintptr_t)offset & 0xFFFF;
    idt[i].offsetHigh = (uintptr_t)offset >> 16;
    idt[i].selector   = GDT::KERNEL_CODE;
    idt[i].zero       = 0;
    idt[i].flags      = 0x8E;
}

void init()
{
    set_gate( 0, exception0);
    set_gate( 1, exception1);
    set_gate( 2, exception2);
    set_gate( 3, exception3);
    set_gate( 4, exception4);
    set_gate( 5, exception5);
    set_gate( 6, exception6);
    set_gate( 7, exception7);
    set_gate( 8, exception8);
    set_gate( 9, exception9);
    set_gate(10, exception10);
    set_gate(11, exception11);
    set_gate(12, exception12);
    set_gate(13, exception13);
    set_gate(14, exception14);
    set_gate(15, exception15);
    set_gate(16, exception16);
    set_gate(17, exception17);
    set_gate(18, exception18);
    set_gate(19, exception19);
    set_gate(20, exception20);
    set_gate(21, exception21);
    set_gate(22, exception22);
    set_gate(23, exception23);
    set_gate(24, exception24);
    set_gate(25, exception25);
    set_gate(26, exception26);
    set_gate(27, exception27);
    set_gate(28, exception28);
    set_gate(29, exception29);
    set_gate(30, exception30);
    set_gate(31, exception31);

    idt_load((uintptr_t)idt, sizeof(idt));
}

}
