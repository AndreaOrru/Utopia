#include "idt.h"
#include "term.h"
#include "x86.h"
#include "irq.h"

#define PIC1_CMD   0x20
#define PIC2_CMD   0xA0
#define PIC1_DATA  0x21
#define PIC2_DATA  0xA1

static State* irq_unhandled(State* state)
{
    printf("\n>>> IRQ %u raised.", state->num);

    return state;
}

IsrHandler irqHandlers[16] = { [0 ... 15] = irq_unhandled };

static void pic_remap(void)
{
    outb(PIC1_CMD,   0x11);  outb(PIC2_CMD,  0x11);
    outb(PIC1_DATA,    32);  outb(PIC2_DATA,   40);
    outb(PIC1_DATA, 0b100);  outb(PIC2_DATA, 0b10);
    outb(PIC1_DATA,  0x01);  outb(PIC2_DATA, 0x01);
    outb(PIC1_DATA,  0xFF);  outb(PIC2_DATA, 0xFF);
}

void irq_mask(uint8_t irq)
{
    uint16_t port = (irq < 8) ? PIC1_DATA : PIC2_DATA;
    outb(port, inb(port) | (1 << irq % 8));
}

void irq_unmask(uint8_t irq)
{
    uint16_t port = (irq < 8) ? PIC1_DATA : PIC2_DATA;
    outb(port, inb(port) & ~(1 << irq % 8));
}

void irq_register(uint8_t n, IsrHandler handler)
{
    irqHandlers[n] = handler;
    irq_unmask(n);
}

void irq_init(void)
{
    pic_remap();

    idt_set_gate(32, irq0);
    idt_set_gate(33, irq1);
    idt_set_gate(34, irq2);
    idt_set_gate(35, irq3);
    idt_set_gate(36, irq4);
    idt_set_gate(37, irq5);
    idt_set_gate(38, irq6);
    idt_set_gate(39, irq7);
    idt_set_gate(40, irq8);
    idt_set_gate(41, irq9);
    idt_set_gate(42, irq10);
    idt_set_gate(43, irq11);
    idt_set_gate(44, irq12);
    idt_set_gate(45, irq13);
    idt_set_gate(46, irq14);
    idt_set_gate(47, irq15);
}
