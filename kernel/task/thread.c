#include "gdt.h"
#include "layout.h"
#include "scheduler.h"
#include "string.h"
#include "pmem.h"
#include "vmem.h"
#include "thread.h"

static Thread* const TCBs = (Thread*)TCB_START;
static uint16_t next_tid = 1;

static TLS* const kernelTLSs = (TLS*)KERNEL_TLS;
static TLS* const   userTLSs = (TLS*)USER_TLS;

alwaysinline Thread* thread_get(uint16_t tid)
{
    return &TCBs[tid];
}

void thread_create(const void* entry, Process* process)
{
    Thread* thread = &TCBs[next_tid];
    map(thread, NULL, PAGE_WRITE | PAGE_GLOBAL);

    thread->tid        = next_tid++;
    thread->state      = READY;
    thread->waitingIrq = 0;
    thread->waitingFor = 0;
    list_init(&thread->waitingList);

    thread->process  = process;
    thread->localTid = process->nextLocalTid++;
    list_append(&process->threads, &thread->processLink);

    void* stack = (void*)USER_STACKS + (thread->localTid * PAGE_SIZE) - 4;
    map(stack, NULL, PAGE_WRITE | PAGE_USER);
    *(void**)stack = (void*)THREAD_MAGIC;

    void* TLS = frame_alloc();
    map(&kernelTLSs[thread->tid],    TLS, PAGE_WRITE | PAGE_GLOBAL);
    map(&userTLSs[thread->localTid], TLS, PAGE_WRITE | PAGE_USER);

    memset(&thread->context, 0, sizeof(Context));
    thread->context.cs  = USER_CODE | USER_RPL;
    thread->context.ss  = USER_DATA | USER_RPL;
    thread->context.eip = (uint32_t)entry;
    thread->context.esp = (uint32_t)stack;
    thread->context.eflags = 0x202;

    scheduler_add(thread);
}

void thread_exit(Thread* thread)
{
    list_remove(&thread->processLink);
    unmap((void*)USER_STACKS + ((thread->localTid - 1) * PAGE_SIZE));

    frame_free(vmem_to_phys(&kernelTLSs[thread->tid]));
    unmap(&kernelTLSs[thread->tid]);
    unmap(&userTLSs[thread->localTid]);

    thread->state = DYING;
    if (scheduler_current() == thread)
        schedule();
}
