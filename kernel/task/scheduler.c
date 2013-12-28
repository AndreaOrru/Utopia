#include "gdt.h"
#include "scheduler.h"

static LIST(readyQueue);

static void schedule(void)
{
    scheduler_add(scheduler_pop());

    Thread* current = scheduler_current();
    if (current->process->PD != read_cr3())
        write_cr3(current->process->PD);

    set_kernel_stack(&current->context + 1);
    set_context(&current->context);
}

alwaysinline void scheduler_add(Thread* thread)
{
    list_append(&readyQueue, &thread->queueLink);
}

alwaysinline Thread* scheduler_current(void)
{
    return list_item(list_first(&readyQueue), Thread, queueLink);
}

alwaysinline Thread* scheduler_pop(void)
{
    return list_item(list_pop(&readyQueue), Thread, queueLink);
}

void scheduler_init(void)
{
    irq_register(0, schedule);
}
