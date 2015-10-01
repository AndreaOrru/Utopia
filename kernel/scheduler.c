#include <assert.h>     // assert.
#include <stddef.h>     // NULL.
#include <vmem.h>       // addrspace_switch.
#include "list.h"       // LIST, list_*.
#include "process.h"    // Process.
#include "timer.h"      // timer_handler_set.
#include "tty.h"        // ERROR.
#include "scheduler.h"

static LIST(ready_queue);
Process* current_process;

inline void schedule_to(Thread* thread)
{
    assert(thread != NULL);

    list_remove(&thread->queue_link);
    list_append(&ready_queue, &thread->queue_link);

    if (thread->process != current_process)
    {
        current_process = thread->process;
        addrspace_switch(current_process->addrspace);
    }

    context_set(&thread->context);
}

void schedule(void)
{
    if (list_empty(&ready_queue))
        ERROR("No more threads to schedule.");

    schedule_to(list_item(list_first(&ready_queue), Thread, queue_link));
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
