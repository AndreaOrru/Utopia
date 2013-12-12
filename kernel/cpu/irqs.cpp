#include "idt.hpp"
#include "term.hpp"
#include "x86.hpp"
#include "irqs.hpp"

namespace IRQs {

extern "C" { IsrHandler irqHandlers[16]; }

void unhandled(InterruptStack stack)
{
    Term::printf("\n*** IRQ %u raised ***", stack.num);
}

const uint16_t PIC1_CMD  = 0x20, PIC2_CMD  = 0xA0;
const uint16_t PIC1_DATA = 0x21, PIC2_DATA = 0xA1;

void pic_init()
{
    outb(PIC1_CMD,   0x11);  outb(PIC2_CMD,  0x11);
    outb(PIC1_DATA,    32);  outb(PIC2_DATA,   40);
    outb(PIC1_DATA, 0b100);  outb(PIC2_DATA, 0b10);
    outb(PIC1_DATA,  0x01);  outb(PIC2_DATA, 0x01);
    outb(PIC1_DATA,  0xFF);  outb(PIC2_DATA, 0xFF);
}

void mask(uint8_t irq)
{
    uint16_t port = (irq < 8) ? PIC1_DATA : PIC2_DATA;
    outb(port, inb(port) | (1 << irq % 8));
}

void unmask(uint8_t irq)
{
    uint16_t port = (irq < 8) ? PIC1_DATA : PIC2_DATA;
    outb(port, inb(port) & ~(1 << irq % 8));
}

void register_handler(uint8_t n, IsrHandler handler)
{
    irqHandlers[n] = handler;
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
        irqHandlers[i] = unhandled;
}

}
