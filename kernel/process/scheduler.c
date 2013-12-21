#include "gdt.h"
#include "irq.h"
#include "string.h"
#include "vmem.h"
#include "scheduler.h"

static uint16_t last_tid = 0;
static Thread*    TCBs =  (Thread*)0xF0000000;
static uint8_t* stacks = (uint8_t*)0xE0000000;

static LIST(activeQueue);
Thread* currentThread = NULL;

void create_thread(void* entry)
{
    Thread* thread = &TCBs[++last_tid];
    uint8_t* stack = stacks + (last_tid * PAGE_SIZE);

    map(thread, NULL, PAGE_WRITE | PAGE_GLOBAL);
    map(stack,  NULL, PAGE_WRITE | PAGE_USER);
    thread->tid = last_tid;

    memset(&thread->state, 0, sizeof(State));
    thread->state.cs = USER_CODE | USER_RPL;
    thread->state.ds = USER_DATA | USER_RPL;
    thread->state.es = USER_DATA | USER_RPL;
    thread->state.ss = USER_DATA | USER_RPL;
    thread->state.eflags = 0x202;
    thread->state.eip = (uint32_t)entry;
    thread->state.esp = (uint32_t)stack;

    list_append(&activeQueue, &thread->queueLink);
}

static State* schedule(unused State* state)
{
    if (currentThread)
        list_append(&activeQueue, &currentThread->queueLink);

    currentThread = list_item(list_pop(&activeQueue), Thread, queueLink);

    set_kernel_stack((uint32_t)(&currentThread->state + 1));
    return &currentThread->state;
}

void scheduler_init(void)
{
    irq_register(0, schedule);
}
