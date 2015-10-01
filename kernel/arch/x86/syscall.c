#include <idt.h>      // idt_gate_set.
#include <isr.h>      // isr128.
#include "syscall.h"

void* syscallHandlers[] = {};

void syscall_init(void)
{
    idt_gate_set(0x80, SYSCALL_GATE, isr128);
}
