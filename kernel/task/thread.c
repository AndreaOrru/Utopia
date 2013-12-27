#include "gdt.h"
#include "ipc.h"
#include "layout.h"
#include "scheduler.h"
#include "string.h"
#include "vmem.h"
#include "thread.h"

static Thread* const TCBs = (Thread*)TCB_START;
UTCB* const kernelUTCBs = (UTCB*)KERNEL_UTCB;
UTCB* const   userUTCBs = (UTCB*)USER_UTCB;

extern Process* volatile currentProcess;
static uint16_t next_tid = 1;

void thread_create(const void* entry)
{
    Thread* thread = &TCBs[next_tid];
    map(thread, NULL, PAGE_WRITE | PAGE_GLOBAL);

    thread->tid      = next_tid++;
    thread->state    = NEW;
    thread->process  = currentProcess;
    thread->localTid = thread->process->nextLocalTid++;
    list_append(&currentProcess->threads, &thread->processLink);

    void* stack = (void*)USER_STACKS + (thread->localTid * PAGE_SIZE) - 4;
    map(stack, NULL, PAGE_WRITE | PAGE_USER);

    map(&kernelUTCBs[thread->tid],      NULL, PAGE_WRITE | PAGE_GLOBAL);
    map(  &userUTCBs[thread->localTid], NULL, PAGE_WRITE | PAGE_USER);

    memset(&thread->context, 0, sizeof(Context));
    thread->context.cs  = USER_CODE | USER_RPL;
    thread->context.ss  = USER_DATA | USER_RPL;
    thread->context.eip = (uint32_t)entry;
    thread->context.esp = (uint32_t)stack;
    thread->context.eflags = 0x202;

    switch_to(thread);
}
