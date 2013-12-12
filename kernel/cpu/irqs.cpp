#include "idt.hpp"
#include "isrs.hpp"
#include "machine.hpp"
#include "term.hpp"
#include "irqs.hpp"

namespace IRQs
{

extern "C" { IsrHandler irqHandlers[16]; }

void unhandled_irq(InterruptStack stack)
{
    Term::printf("\n*** IRQ %u raised ***\n", stack.num);
    hlt();
}

void pic_init()
{
    const uint16_t PIC1_CMD  = 0x20, PIC2_CMD  = 0xA0;
    const uint16_t PIC1_DATA = 0x21, PIC2_DATA = 0xA1;

    outb(PIC1_CMD,   0x11);  outb(PIC2_CMD,  0x11);
    outb(PIC1_DATA,    32);  outb(PIC2_DATA,   40);
    outb(PIC1_DATA, 0b100);  outb(PIC2_DATA, 0b10);
    outb(PIC1_DATA,  0x01);  outb(PIC2_DATA, 0x01);
    outb(PIC1_DATA,  0x00);  outb(PIC2_DATA, 0x00);
}

void init()
{
    pic_init();

    using IDT::set_gate;

    set_gate(32, irq0);
    set_gate(33, irq1);
    set_gate(34, irq2);
    set_gate(35, irq3);
    set_gate(36, irq4);
    set_gate(37, irq5);
    set_gate(38, irq6);
    set_gate(39, irq7);
    set_gate(40, irq8);
    set_gate(41, irq9);
    set_gate(42, irq10);
    set_gate(43, irq11);
    set_gate(44, irq12);
    set_gate(45, irq13);
    set_gate(46, irq14);
    set_gate(47, irq15);

    for (int i = 0; i < 32; i++)
        irqHandlers[i] = unhandled_irq;
}

}
