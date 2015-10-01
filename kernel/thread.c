#include <assert.h>     // assert.
#include <interrupt.h>  // context_init.
#include <layout.h>     // TCB_START, THREAD_EXIT, USER_STACKS.
#include <vmem.h>       // addrspace_current, map, PAGE_*.
#include "thread.h"

static Thread* const TCBs = (Thread*) TCB_START;
static uint16_t next_tid = 1;

void thread_create(const void* entry, Process* process)
{
    assert(entry != NULL);
    assert(process != NULL);
    assert(process->addrspace == addrspace_current());

    Thread* thread = &TCBs[next_tid];
    map(thread, NULL, PAGE_WRITE | PAGE_GLOBAL);

    thread->tid = next_tid++;
    thread->process = process;
    thread->local_tid = process->next_local_tid++;
    list_append(&process->threads, &thread->process_link);

    void* stack = (void*) USER_STACKS + 2*(thread->local_tid * PAGE_SIZE) - sizeof(int);
    map(stack, NULL, PAGE_WRITE | PAGE_USER);
    *((void**) stack) = (void*) THREAD_EXIT;

    context_init(&thread->context, entry, stack);
}
