#include "idt.h"
#include "ipc.h"
#include "isr.h"
#include "process.h"
#include "term.h"
#include "syscall.h"

void* syscallHandlers[] =
{
    [0] = put,
    [1] = send_receive,
    [2] = sbrk,
};

void syscall_init(void)
{
    idt_set_gate(0x80, SYSCALL_GATE, isr128);
}
