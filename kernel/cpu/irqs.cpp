#include "idt.hpp"
#include "term.hpp"
#include "x86.hpp"
#include "irqs.hpp"

namespace IRQs {

extern "C" { IsrHandler irqHandlers[16]; }

void unhandled(InterruptStack* stack)
{
    Term::printf("\n>>> IRQ %u raised.", stack->num);
}

const uint16_t PIC1_CMD  = 0x20, PIC2_CMD  = 0xA0;
const uint16_t PIC1_DATA = 0x21, PIC2_DATA = 0xA1;

void remap_pic()
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

void register_handler(uint8_t n, IsrHandler handler, bool unmask)
{
    irqHandlers[n] = handler;
    if (unmask) IRQs::unmask(n);
}

void init()
{
    remap_pic();

    using IDT::set_gate;

    set_gate(32, _irq0);
    set_gate(33, _irq1);
    set_gate(34, _irq2);
    set_gate(35, _irq3);
    set_gate(36, _irq4);
    set_gate(37, _irq5);
    set_gate(38, _irq6);
    set_gate(39, _irq7);
    set_gate(40, _irq8);
    set_gate(41, _irq9);
    set_gate(42, _irq10);
    set_gate(43, _irq11);
    set_gate(44, _irq12);
    set_gate(45, _irq13);
    set_gate(46, _irq14);
    set_gate(47, _irq15);

    for (int i = 0; i < 16; i++)
        irqHandlers[i] = unhandled;
}

}
