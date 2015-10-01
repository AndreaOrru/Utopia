#include <assert.h>     // assert.
#include <stddef.h>     // NULL.
#include <vmem.h>       // addrspace_*.
#include "list.h"       // LIST, list_*.
#include "timer.h"      // timer_handler_set.
#include "tty.h"        // ERROR.
#include "scheduler.h"

static LIST(ready_queue);

static inline void thread_switch(Thread* thread)
{
    assert(thread != NULL);

    if (thread->process->addrspace != addrspace_current())
        addrspace_switch(thread->process->addrspace);

    context_set(&thread->context);
}

void schedule(void)
{
    if (list_empty(&ready_queue))
        ERROR("No more threads to schedule.");

    Thread* next = list_item(list_pop(&ready_queue), Thread, queue_link);
    list_append(&ready_queue, &next->queue_link);

    thread_switch(scheduler_current());
}

inline void scheduler_add(Thread* thread)
{
    assert(thread != NULL);

    list_prepend(&ready_queue, &thread->queue_link);
}

inline void scheduler_remove(Thread* thread)
{
    assert(thread != NULL);
    bool current = (list_last(&ready_queue) == &thread->queue_link);

    list_remove(&thread->queue_link);
    if (current)
        schedule();
}

inline Thread* scheduler_current(void)
{
    return list_item(list_last(&ready_queue), Thread, queue_link);
}

void scheduler_init(void)
{
    timer_handler_set(schedule);
}
