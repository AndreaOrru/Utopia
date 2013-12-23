#include "idt.h"
#include "term.h"
#include "x86.h"
#include "isr.h"

#define PIC1_CMD   0x20
#define PIC2_CMD   0xA0
#define PIC1_DATA  0x21
#define PIC2_DATA  0xA1

static const char* const interruptNames[] =
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

static State* isr_unhandled(State* state)
{
    if (state->num < 32)
        printf("\n>>> Exception: %s.", interruptNames[state->num]);
    else if (state->num < 32 + 16)
        printf("\n>>> IRQ: %s", interruptNames[state->num]);
    else
        printf("\n>>> Invalid interrupt.");

    hlt();
    return state;
}

IsrHandler isrHandlers[256] = { [0 ... 255] = isr_unhandled };

void isr_register(uint8_t n, IsrHandler handler)
{
    isrHandlers[n] = handler;
}

void irq_register(uint8_t n, IsrHandler handler)
{
    isrHandlers[32 + n] = handler;
    irq_unmask(n);
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

static void pic_remap(void)
{
    outb(PIC1_CMD,   0x11);  outb(PIC2_CMD,  0x11);
    outb(PIC1_DATA,    32);  outb(PIC2_DATA,   40);
    outb(PIC1_DATA, 0b100);  outb(PIC2_DATA, 0b10);
    outb(PIC1_DATA,  0x01);  outb(PIC2_DATA, 0x01);
    outb(PIC1_DATA,  0xFF);  outb(PIC2_DATA, 0xFF);
}

void isr_init(void)
{
    pic_remap();

    idt_set_gate(0,  isr0);
    idt_set_gate(1,  isr1);
    idt_set_gate(2,  isr2);
    idt_set_gate(3,  isr3);
    idt_set_gate(4,  isr4);
    idt_set_gate(5,  isr5);
    idt_set_gate(6,  isr6);
    idt_set_gate(7,  isr7);
    idt_set_gate(8,  isr8);
    idt_set_gate(9,  isr9);
    idt_set_gate(10, isr10);
    idt_set_gate(11, isr11);
    idt_set_gate(12, isr12);
    idt_set_gate(13, isr13);
    idt_set_gate(14, isr14);
    idt_set_gate(15, isr15);
    idt_set_gate(16, isr16);
    idt_set_gate(17, isr17);
    idt_set_gate(18, isr18);
    idt_set_gate(19, isr19);
    idt_set_gate(20, isr20);
    idt_set_gate(21, isr21);
    idt_set_gate(22, isr22);
    idt_set_gate(23, isr23);
    idt_set_gate(24, isr24);
    idt_set_gate(25, isr25);
    idt_set_gate(26, isr26);
    idt_set_gate(27, isr27);
    idt_set_gate(28, isr28);
    idt_set_gate(29, isr29);
    idt_set_gate(30, isr30);
    idt_set_gate(31, isr31);

    idt_set_gate(32, isr32);
    idt_set_gate(33, isr33);
    idt_set_gate(34, isr34);
    idt_set_gate(35, isr35);
    idt_set_gate(36, isr36);
    idt_set_gate(37, isr37);
    idt_set_gate(38, isr38);
    idt_set_gate(39, isr39);
    idt_set_gate(40, isr40);
    idt_set_gate(41, isr41);
    idt_set_gate(42, isr42);
    idt_set_gate(43, isr43);
    idt_set_gate(44, isr44);
    idt_set_gate(45, isr45);
    idt_set_gate(46, isr46);
    idt_set_gate(47, isr47);

    for (int i = 48; i < 256; i++)
        idt_set_gate(i, isr255);
}
