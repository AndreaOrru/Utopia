#include "idt.h"
#include "isr.h"
#include "x86.h"
#include "syscall.h"

static State* syscall(State* state)
{
    printf("Syscall!");

    hlt();
    return state;
}

void syscall_init(void)
{
    idt_set_gate(0x80, SYSCALL_GATE, isr128);
    isr_register(0x80, syscall);
}
