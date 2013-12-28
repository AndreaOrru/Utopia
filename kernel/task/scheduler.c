#include "gdt.h"
#include "scheduler.h"

static LIST(readyQueue);

static inline void switch_to(Thread* thread)
{
    if (thread->process->PD != read_cr3())
        write_cr3(thread->process->PD);

    set_kernel_stack(&thread->context + 1);
    set_context(&thread->context);
}

static void schedule(void)
{
    scheduler_add(scheduler_pop());
    switch_to(scheduler_current());
}

alwaysinline void scheduler_add(Thread* thread)
{
    list_append(&readyQueue, &thread->queueLink);
}

alwaysinline Thread* scheduler_current(void)
{
    return list_item(list_first(&readyQueue), Thread, queueLink);
}

inline Thread* scheduler_pop(void)
{
    Thread* thread = list_item(list_pop(&readyQueue), Thread, queueLink);
    switch_to(scheduler_current());

    return thread;
}

void scheduler_init(void)
{
    irq_register(0, schedule);
}
