#include "gdt.h"
#include "irq.h"
#include "string.h"
#include "vmem.h"
#include "scheduler.h"

static uint16_t lastID = 0;
static Thread*    TCBs =  (Thread*)0xF0000000;
static uint8_t* stacks = (uint8_t*)0xE0000000;

static LIST(activeQueue);
Thread* currentThread = NULL;

void create_process(ElfHeader* elf)
{
    void* PD = new_address_space();
    write_cr3(PD);

    Thread* process = create_thread(elf_load(elf));
    process->parent = (union Thread*)process;
    process->PD     = PD;
}

Thread* create_thread(const void* entry)
{
    Thread* thread = &TCBs[++lastID];
    uint8_t* stack = stacks + (lastID * PAGE_SIZE) - 4;

    map(thread, NULL, PAGE_WRITE | PAGE_GLOBAL);
    map(stack,  NULL, PAGE_WRITE | PAGE_USER);

    thread->tid = lastID;
    list_init(&thread->childs);
    if (currentThread)
    {
        thread->PD = currentThread->PD;
        Thread* parent = (Thread*)currentThread->parent;
        thread->parent = (union Thread*)parent;
        list_append(&parent->childs, &thread->childLink);
    }

    memset(&thread->state, 0, sizeof(State));
    thread->state.cs = USER_CODE | USER_RPL;
    thread->state.ds = USER_DATA | USER_RPL;
    thread->state.es = USER_DATA | USER_RPL;
    thread->state.ss = USER_DATA | USER_RPL;
    thread->state.eflags = 0x202;
    thread->state.eip = (uint32_t)entry;
    thread->state.esp = (uint32_t)stack;

    list_append(&activeQueue, &thread->queueLink);
    return thread;
}

static State* schedule(unused State* state)
{
    if (currentThread)
        list_append(&activeQueue, &currentThread->queueLink);
    currentThread = list_item(list_pop(&activeQueue), Thread, queueLink);

    if (read_cr3() != currentThread->PD)
        write_cr3(currentThread->PD);

    set_kernel_stack(&currentThread->state + 1);
    return &currentThread->state;
}

void scheduler_init(void)
{
    irq_register(0, schedule);
}
