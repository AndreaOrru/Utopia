#include "idt.h"
#include "isr.h"
#include "term.h"
#include "x86.h"
#include "interrupt.h"

#define PIC1_CMD   0x20
#define PIC2_CMD   0xA0
#define PIC1_DATA  0x21
#define PIC2_DATA  0xA1

State* state;

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

static void interrupt_unhandled(void)
{
    State* state = get_state();

    if (state->num < 32)
        printf("\n>>> Exception: %s.", interruptNames[state->num]);
    else if (state->num < 32 + 16)
        printf("\n>>> IRQ: %s", interruptNames[state->num]);

    hlt();
}

InterruptHandler interruptHandlers[32 + 16] = { [0 ... 47] = interrupt_unhandled };

void interrupt_register(uint8_t n, InterruptHandler handler)
{
    interruptHandlers[n] = handler;
}

void irq_register(uint8_t n, InterruptHandler handler)
{
    interruptHandlers[32 + n] = handler;
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

alwaysinline State* get_state(void)
{
    return state;
}

alwaysinline void set_state(State* newState)
{
    state = newState;
}

void interrupt_init(void)
{
    pic_remap();

    idt_set_gate(0,  INTERRUPT_GATE, isr0);
    idt_set_gate(1,  INTERRUPT_GATE, isr1);
    idt_set_gate(2,  INTERRUPT_GATE, isr2);
    idt_set_gate(3,  INTERRUPT_GATE, isr3);
    idt_set_gate(4,  INTERRUPT_GATE, isr4);
    idt_set_gate(5,  INTERRUPT_GATE, isr5);
    idt_set_gate(6,  INTERRUPT_GATE, isr6);
    idt_set_gate(7,  INTERRUPT_GATE, isr7);
    idt_set_gate(8,  INTERRUPT_GATE, isr8);
    idt_set_gate(9,  INTERRUPT_GATE, isr9);
    idt_set_gate(10, INTERRUPT_GATE, isr10);
    idt_set_gate(11, INTERRUPT_GATE, isr11);
    idt_set_gate(12, INTERRUPT_GATE, isr12);
    idt_set_gate(13, INTERRUPT_GATE, isr13);
    idt_set_gate(14, INTERRUPT_GATE, isr14);
    idt_set_gate(15, INTERRUPT_GATE, isr15);
    idt_set_gate(16, INTERRUPT_GATE, isr16);
    idt_set_gate(17, INTERRUPT_GATE, isr17);
    idt_set_gate(18, INTERRUPT_GATE, isr18);
    idt_set_gate(19, INTERRUPT_GATE, isr19);
    idt_set_gate(20, INTERRUPT_GATE, isr20);
    idt_set_gate(21, INTERRUPT_GATE, isr21);
    idt_set_gate(22, INTERRUPT_GATE, isr22);
    idt_set_gate(23, INTERRUPT_GATE, isr23);
    idt_set_gate(24, INTERRUPT_GATE, isr24);
    idt_set_gate(25, INTERRUPT_GATE, isr25);
    idt_set_gate(26, INTERRUPT_GATE, isr26);
    idt_set_gate(27, INTERRUPT_GATE, isr27);
    idt_set_gate(28, INTERRUPT_GATE, isr28);
    idt_set_gate(29, INTERRUPT_GATE, isr29);
    idt_set_gate(30, INTERRUPT_GATE, isr30);
    idt_set_gate(31, INTERRUPT_GATE, isr31);

    idt_set_gate(32, INTERRUPT_GATE, isr32);
    idt_set_gate(33, INTERRUPT_GATE, isr33);
    idt_set_gate(34, INTERRUPT_GATE, isr34);
    idt_set_gate(35, INTERRUPT_GATE, isr35);
    idt_set_gate(36, INTERRUPT_GATE, isr36);
    idt_set_gate(37, INTERRUPT_GATE, isr37);
    idt_set_gate(38, INTERRUPT_GATE, isr38);
    idt_set_gate(39, INTERRUPT_GATE, isr39);
    idt_set_gate(40, INTERRUPT_GATE, isr40);
    idt_set_gate(41, INTERRUPT_GATE, isr41);
    idt_set_gate(42, INTERRUPT_GATE, isr42);
    idt_set_gate(43, INTERRUPT_GATE, isr43);
    idt_set_gate(44, INTERRUPT_GATE, isr44);
    idt_set_gate(45, INTERRUPT_GATE, isr45);
    idt_set_gate(46, INTERRUPT_GATE, isr46);
    idt_set_gate(47, INTERRUPT_GATE, isr47);
}
