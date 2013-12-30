#include "gdt.h"
#include "ipc.h"
#include "layout.h"
#include "scheduler.h"
#include "term.h"
#include "vmem.h"

static LIST(readyQueue);
static UTCB** const UTCBPtr = (UTCB**)UTCB_PTR;

static inline void switch_to(Thread* thread)
{
    if (thread->process->PD != read_cr3())
        write_cr3(thread->process->PD);

    *UTCBPtr = (UTCB*)USER_UTCB + thread->localTid;
    set_kernel_stack(&thread->context + 1);
    set_context(&thread->context);
}

static void schedule(void)
{
    if (list_empty(&readyQueue))
    {
        printf("\n>>> No more threads.");
        hlt();
    }

    scheduler_add(list_item(list_pop(&readyQueue), Thread, queueLink));
    switch_to(scheduler_current());
}

alwaysinline void scheduler_add(Thread* thread)
{
    list_insert_before(&scheduler_current()->queueLink, &thread->queueLink);
}

alwaysinline void scheduler_remove(Thread* thread)
{
    bool current = (list_last(&readyQueue) == &thread->queueLink);

    list_remove(&thread->queueLink);
    if (current)
        schedule();
}

alwaysinline Thread* scheduler_current(void)
{
    return list_item(list_last(&readyQueue), Thread, queueLink);
}

void scheduler_init(void)
{
    map(UTCBPtr, NULL, PAGE_WRITE | PAGE_USER | PAGE_GLOBAL);

    irq_register(0, schedule);
}
