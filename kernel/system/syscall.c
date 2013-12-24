#include "idt.h"
#include "isr.h"
#include "term.h"
#include "syscall.h"

void* syscallHandlers[] =
{
    [0] = printf,
};

void syscall_init(void)
{
    idt_set_gate(0x80, SYSCALL_GATE, isr128);
}
