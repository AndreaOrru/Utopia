#include "gdt.h"
#include "scheduler.h"

static LIST(readyQueue);
Process* volatile currentProcess;
Thread*  volatile currentThread;

void switch_to(Thread* thread)
{
    if (currentThread && currentThread->state == READY)
        list_append(&readyQueue, &currentThread->queueLink);

    if (thread->state == READY)
        list_remove(&thread->queueLink);
    else
        thread->state = READY;

    currentThread  = thread;
    currentProcess = thread->process;

    if (read_cr3() != currentProcess->PD)
        write_cr3(currentProcess->PD);

    set_kernel_stack(&thread->context + 1);
    set_context(&thread->context);
}

static void schedule(void)
{
    switch_to(list_item(list_first(&readyQueue), Thread, queueLink));
}

void scheduler_init(void)
{
    irq_register(0, schedule);
}
