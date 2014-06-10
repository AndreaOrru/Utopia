#include "gdt.h"
#include "ipc.h"
#include "layout.h"
#include "scheduler.h"
#include "term.h"
#include "vmem.h"

static LIST(readyQueue);
static TLS* volatile* const TLSPtr = (TLS**)TLS_PTR;

static inline void switch_to(Thread* thread)
{
    if (thread->process->PD != read_cr3())
        write_cr3(thread->process->PD);

    *TLSPtr = (TLS*)USER_TLS + thread->localTid;
    set_kernel_stack(&thread->context + 1);
    set_context(&thread->context);
}

void schedule(void)
{
  retry:
    if (list_empty(&readyQueue))
        ERROR("No more threads to schedule.");

    Thread* new = list_item(list_pop(&readyQueue), Thread, queueLink);
    if (new->state == DYING)
    {
        unmap(new);
        goto retry;
    }

    list_append(&readyQueue, &new->queueLink);
    switch_to(scheduler_current());
}

alwaysinline void scheduler_add(Thread* thread)
{
    list_prepend(&readyQueue, &thread->queueLink);
}

inline void scheduler_remove(Thread* thread)
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

void scheduler_wait(uint16_t waitFor, State reason)
{
    Thread* thread = scheduler_current();
    scheduler_remove(thread);

    thread->state = reason;

    if (reason == WAIT_SENDING)
        list_append(&thread_get(waitFor)->waitingList, &thread->queueLink);

    else if (reason == WAIT_RECEIVING)
        thread->waitingFor = waitFor;

    else
    {
        thread->waitingFor = waitFor;
        list_append(&thread_get(waitFor)->waitingList, &thread->queueLink);
    }
}

inline void scheduler_unblock(Thread* thread)
{
    thread->state = READY;
    scheduler_add(thread);
}

void scheduler_init(void)
{
    map((void*)TLSPtr, NULL, PAGE_WRITE | PAGE_USER | PAGE_GLOBAL);

    irq_register(0, schedule);
}
