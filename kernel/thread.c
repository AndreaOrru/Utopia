#include <assert.h>     // assert.
#include <interrupt.h>  // context_init.
#include <layout.h>     // TCB_START, THREAD_EXIT, USER_STACKS.
#include <vmem.h>       // addrspace_current, map, unmap, PAGE_*.
#include "scheduler.h"  // scheduler_add, scheduler_current.
#include "thread.h"

extern Process* current_process;

static Thread* const TCBs = (Thread*) TCB_START;
static uint16_t next_tid = 1;

Thread* thread_create(const void* entry)
{
    assert(entry != NULL);

    Thread* thread = &TCBs[next_tid];
    map(thread, NULL, PAGE_WRITE | PAGE_GLOBAL);

    thread->tid = next_tid++;
    thread->process = current_process;
    thread->local_tid = current_process->next_local_tid++;
    list_append(&current_process->threads, &thread->process_link);

    void* stack = (void*) USER_STACKS + 2*(thread->local_tid * PAGE_SIZE) - sizeof(int);
    map(stack, NULL, PAGE_WRITE | PAGE_USER);
    *((void**) stack) = (void*) THREAD_EXIT;

    context_init(&thread->context, entry, stack);

    return thread;
}

void thread_exit(Thread* thread)
{
    if (thread == NULL)
        thread = scheduler_current();

    scheduler_remove(thread);
    list_remove(&thread->process_link);

    if (list_empty(&current_process->threads))
        unmap(current_process);
    unmap((void*) USER_STACKS + 2*((thread->local_tid - 1) * PAGE_SIZE));
    unmap(thread);
}
