#include "idt.h"
#include "isr.h"
#include "term.h"
#include "x86.h"
#include "syscall.h"

static State* syscall_undefined(State* state)
{
    printf("\n>>> Undefined syscall %u.", state->eax);

    hlt();
    return state;
}

InterruptHandler syscallHandlers[64] = { [0 ... 63] = syscall_undefined };

void syscall_register(uint8_t n, InterruptHandler handler)
{
    syscallHandlers[n] = handler;
}

void syscall_init(void)
{
    idt_set_gate(0x80, SYSCALL_GATE, isr128);
}
