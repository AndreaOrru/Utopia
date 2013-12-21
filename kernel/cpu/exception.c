#include "idt.h"
#include "term.h"
#include "x86.h"
#include "exception.h"

static const char* const names[] =
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
    "Reserved"
};

static State* exception_unhandled(State* state)
{
    printf("\n>>> %s.", names[state->num]);
    hlt();

    return state;
}

IsrHandler exceptionHandlers[32] = { [0 ... 31] = exception_unhandled };

void exception_register(uint8_t n, IsrHandler handler)
{
    exceptionHandlers[n] = handler;
}

void exception_init(void)
{
    idt_set_gate(0,  exception0);
    idt_set_gate(1,  exception1);
    idt_set_gate(2,  exception2);
    idt_set_gate(3,  exception3);
    idt_set_gate(4,  exception4);
    idt_set_gate(5,  exception5);
    idt_set_gate(6,  exception6);
    idt_set_gate(7,  exception7);
    idt_set_gate(8,  exception8);
    idt_set_gate(9,  exception9);
    idt_set_gate(10, exception10);
    idt_set_gate(11, exception11);
    idt_set_gate(12, exception12);
    idt_set_gate(13, exception13);
    idt_set_gate(14, exception14);
    idt_set_gate(15, exception15);
    idt_set_gate(16, exception16);
    idt_set_gate(17, exception17);
    idt_set_gate(18, exception18);
    idt_set_gate(19, exception19);
    idt_set_gate(20, exception20);
    idt_set_gate(21, exception21);
    idt_set_gate(22, exception22);
    idt_set_gate(23, exception23);
    idt_set_gate(24, exception24);
    idt_set_gate(25, exception25);
    idt_set_gate(26, exception26);
    idt_set_gate(27, exception27);
    idt_set_gate(28, exception28);
    idt_set_gate(29, exception29);
    idt_set_gate(30, exception30);
    idt_set_gate(31, exception31);
}
