#include <assert.h>              // assert.
#include <stddef.h>              // NULL.
#include "tty.h"                 // ERROR.
#include "arch/x86/asm.h"        // inb, outb.
#include "arch/x86/idt.h"        // idt_gate_set, INTERRUPT_GATE.
#include "arch/x86/isr.h"        // isr*.
#include "arch/x86/interrupt.h"

#define PIC1_CMD   0x20
#define PIC2_CMD   0xA0
#define PIC1_DATA  0x21
#define PIC2_DATA  0xA1

Context* volatile context;

void context_set(Context* new_context)
{
    assert(new_context != NULL);

    context = new_context;
}

Context* context_get(void)
{
    return context;
}

static const char* const interrupt_names[] =
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating Point Exception",
    "Virtualization Exception",
    "Reserved", "Reserved", "Reserved",
    "Reserved", "Reserved", "Reserved",
    "Reserved", "Reserved", "Reserved",
    "Security Exception",
    "Reserved",

    "Programmable Interrupt Timer",
    "Keyboard",
    "Cascade",
    "COM2",
    "COM1",
    "LPT2",
    "Floppy Disk",
    "LPT1",
    "CMOS RTC",
    "Legacy SCSI / NIC / ?",
    "SCSI / NIC / ?",
    "SCSI / NIC / ?",
    "PS2 Mouse",
    "FPU",
    "Primary ATA Hard Disk",
    "Secondary ATA Hard Disk"
};

static void interrupt_unhandled(void)
{
    Context* context = context_get();
    assert(context != NULL);

    if (context->int_n < 32)
        ERROR("Exception: %s.", interrupt_names[context->int_n]);
    else if (context->int_n < 32 + 16)
        ERROR("IRQ: %s", interrupt_names[context->int_n]);
}

InterruptHandler interrupt_handlers[32 + 16] = { [0 ... 47] = interrupt_unhandled };

void interrupt_register(uint8_t n, InterruptHandler handler)
{
    assert(n < 48);
    assert(handler != NULL);

    interrupt_handlers[n] = handler;
}

void irq_register(uint8_t irq, InterruptHandler handler)
{
    assert(irq < 16);
    assert(handler != NULL);

    interrupt_handlers[32 + irq] = handler;
    irq_unmask(irq);
}

void irq_mask(uint8_t irq)
{
    assert(irq < 16);

    uint16_t port = (irq < 8) ? PIC1_DATA : PIC2_DATA;
    outb(port, inb(port) | (1 << irq%8));
}

void irq_unmask(uint8_t irq)
{
    assert(irq < 16);

    uint16_t port = (irq < 8) ? PIC1_DATA : PIC2_DATA;
    outb(port, inb(port) & ~(1 << irq%8));
}

static void pic_remap(void)
{
    outb(PIC1_CMD,    0x11);  outb(PIC2_CMD,    0x11);
    outb(PIC1_DATA,     32);  outb(PIC2_DATA,     40);
    outb(PIC1_DATA, 1 << 2);  outb(PIC2_DATA, 1 << 1);
    outb(PIC1_DATA,   0x01);  outb(PIC2_DATA,   0x01);
    outb(PIC1_DATA,   0xFF);  outb(PIC2_DATA,   0xFF);
}

void interrupt_init(void)
{
    pic_remap();

    idt_gate_set(0,  INTERRUPT_GATE, isr0);
    idt_gate_set(1,  INTERRUPT_GATE, isr1);
    idt_gate_set(2,  INTERRUPT_GATE, isr2);
    idt_gate_set(3,  INTERRUPT_GATE, isr3);
    idt_gate_set(4,  INTERRUPT_GATE, isr4);
    idt_gate_set(5,  INTERRUPT_GATE, isr5);
    idt_gate_set(6,  INTERRUPT_GATE, isr6);
    idt_gate_set(7,  INTERRUPT_GATE, isr7);
    idt_gate_set(8,  INTERRUPT_GATE, isr8);
    idt_gate_set(9,  INTERRUPT_GATE, isr9);
    idt_gate_set(10, INTERRUPT_GATE, isr10);
    idt_gate_set(11, INTERRUPT_GATE, isr11);
    idt_gate_set(12, INTERRUPT_GATE, isr12);
    idt_gate_set(13, INTERRUPT_GATE, isr13);
    idt_gate_set(14, INTERRUPT_GATE, isr14);
    idt_gate_set(15, INTERRUPT_GATE, isr15);
    idt_gate_set(16, INTERRUPT_GATE, isr16);
    idt_gate_set(17, INTERRUPT_GATE, isr17);
    idt_gate_set(18, INTERRUPT_GATE, isr18);
    idt_gate_set(19, INTERRUPT_GATE, isr19);
    idt_gate_set(20, INTERRUPT_GATE, isr20);
    idt_gate_set(21, INTERRUPT_GATE, isr21);
    idt_gate_set(22, INTERRUPT_GATE, isr22);
    idt_gate_set(23, INTERRUPT_GATE, isr23);
    idt_gate_set(24, INTERRUPT_GATE, isr24);
    idt_gate_set(25, INTERRUPT_GATE, isr25);
    idt_gate_set(26, INTERRUPT_GATE, isr26);
    idt_gate_set(27, INTERRUPT_GATE, isr27);
    idt_gate_set(28, INTERRUPT_GATE, isr28);
    idt_gate_set(29, INTERRUPT_GATE, isr29);
    idt_gate_set(30, INTERRUPT_GATE, isr30);
    idt_gate_set(31, INTERRUPT_GATE, isr31);

    idt_gate_set(32, INTERRUPT_GATE, isr32);
    idt_gate_set(33, INTERRUPT_GATE, isr33);
    idt_gate_set(34, INTERRUPT_GATE, isr34);
    idt_gate_set(35, INTERRUPT_GATE, isr35);
    idt_gate_set(36, INTERRUPT_GATE, isr36);
    idt_gate_set(37, INTERRUPT_GATE, isr37);
    idt_gate_set(38, INTERRUPT_GATE, isr38);
    idt_gate_set(39, INTERRUPT_GATE, isr39);
    idt_gate_set(40, INTERRUPT_GATE, isr40);
    idt_gate_set(41, INTERRUPT_GATE, isr41);
    idt_gate_set(42, INTERRUPT_GATE, isr42);
    idt_gate_set(43, INTERRUPT_GATE, isr43);
    idt_gate_set(44, INTERRUPT_GATE, isr44);
    idt_gate_set(45, INTERRUPT_GATE, isr45);
    idt_gate_set(46, INTERRUPT_GATE, isr46);
    idt_gate_set(47, INTERRUPT_GATE, isr47);
}
