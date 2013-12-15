#include "idt.hpp"
#include "term.hpp"
#include "x86.hpp"
#include "exceptions.hpp"

namespace Exceptions {

const char* names[] =
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

extern "C" { IsrHandler exceptionHandlers[32]; }

void unhandled(InterruptStack stack)
{
    Term::printf("\n*** %s ***", names[stack.num]);
    hlt();
}

void register_handler(uint8_t n, IsrHandler handler)
{
    exceptionHandlers[n] = handler;
}

void init()
{
    using IDT::set_gate;

    set_gate(0,  _exception0);
    set_gate(1,  _exception1);
    set_gate(2,  _exception2);
    set_gate(3,  _exception3);
    set_gate(4,  _exception4);
    set_gate(5,  _exception5);
    set_gate(6,  _exception6);
    set_gate(7,  _exception7);
    set_gate(8,  _exception8);
    set_gate(9,  _exception9);
    set_gate(10, _exception10);
    set_gate(11, _exception11);
    set_gate(12, _exception12);
    set_gate(13, _exception13);
    set_gate(14, _exception14);
    set_gate(15, _exception15);
    set_gate(16, _exception16);
    set_gate(17, _exception17);
    set_gate(18, _exception18);
    set_gate(19, _exception19);
    set_gate(20, _exception20);
    set_gate(21, _exception21);
    set_gate(22, _exception22);
    set_gate(23, _exception23);
    set_gate(24, _exception24);
    set_gate(25, _exception25);
    set_gate(26, _exception26);
    set_gate(27, _exception27);
    set_gate(28, _exception28);
    set_gate(29, _exception29);
    set_gate(30, _exception30);
    set_gate(31, _exception31);

    for (int i = 0; i < 32; i++)
        exceptionHandlers[i] = unhandled;
}

}
