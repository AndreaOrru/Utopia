#include "gdt.h"
#include "layout.h"
#include "scheduler.h"
#include "string.h"
#include "pmem.h"
#include "vmem.h"
#include "thread.h"

static Thread* const TCBs = (Thread*)TCB_START;
static uint16_t next_tid = 1;

static UTCB* const kernelUTCBs = (UTCB*)KERNEL_UTCB;
static UTCB* const   userUTCBs = (UTCB*)USER_UTCB;

alwaysinline Thread* thread_get(uint16_t tid)
{
    return &TCBs[tid];
}

void thread_create(const void* entry, Process* process)
{
    Thread* thread = &TCBs[next_tid];
    map(thread, NULL, PAGE_WRITE | PAGE_GLOBAL);

    thread->tid         = next_tid++;
    thread->state       = NEW;
    thread->listeningTo = 0;
    list_init(&thread->waitingList);

    thread->process  = process;
    thread->localTid = process->nextLocalTid++;
    list_append(&process->threads, &thread->processLink);

    void* stack = (void*)USER_STACKS + (thread->localTid * PAGE_SIZE) - 4;
    map(stack, NULL, PAGE_WRITE | PAGE_USER);

    void* UTCB = frame_alloc();
    map(&kernelUTCBs[thread->tid],    UTCB, PAGE_WRITE | PAGE_GLOBAL);
    map(&userUTCBs[thread->localTid], UTCB, PAGE_WRITE | PAGE_USER);

    memset(&thread->context, 0, sizeof(Context));
    thread->context.cs  = USER_CODE | USER_RPL;
    thread->context.ss  = USER_DATA | USER_RPL;
    thread->context.eip = (uint32_t)entry;
    thread->context.esp = (uint32_t)stack;
    thread->context.eflags = 0x202;

    scheduler_add(thread);
}
