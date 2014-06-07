#include "drivers.h"
#include "idt.h"
#include "ipc.h"
#include "isr.h"
#include "process.h"
#include "scheduler.h"
#include "term.h"
#include "vmem.h"
#include "x86.h"
#include "syscall.h"

void _thread_create(void* entry)
{
    thread_create(entry, scheduler_current()->process);
}

void* syscallHandlers[] =
{
    [0] = put,
    [1] = send_receive,
    [2] = sbrk,
    [3] = irq_subscribe,
    [4] = irq_wait,
    [5] = inb,
    [6] = _thread_create,
    [7] = process_exit,
    [8] = memory_map
};

void syscall_init(void)
{
    idt_set_gate(0x80, SYSCALL_GATE, isr128);
}
