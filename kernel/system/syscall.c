#include "idt.h"
#include "interrupt.h"
#include "isr.h"
#include "term.h"
#include "syscall.h"

typedef uint32_t (*Syscall)();

static void* syscallHandlers[] = { [0] = put };

void syscall_handler(void)
{
    State* state = get_state();

    Syscall function = syscallHandlers[state->eax];
    state->eax = function(state->ebx, state->ecx, state->edx);
}

void syscall_init(void)
{
    idt_set_gate(0x80, SYSCALL_GATE, isr128);
}
