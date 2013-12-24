#include "gdt.h"
#include "layout.h"
#include "string.h"
#include "vmem.h"
#include "scheduler.h"

static uint16_t next_tid = 1;
static uint16_t next_pid = 1;
static Thread*    TCBs =  (Thread*)TCB_START;
static Process*   PCBs = (Process*)PCB_START;
static uint8_t* stacks = (uint8_t*)USER_STACKS;

static LIST(activeQueue);
Process* volatile currentProcess = NULL;
Thread*  volatile currentThread  = NULL;

void create_process(ElfHeader* elf)
{
    Process* process = &PCBs[next_pid];
    map(process, NULL, PAGE_WRITE | PAGE_GLOBAL);

    process->pid = next_pid++;
    process->PD  = new_address_space();
    list_init(&process->threads);

    write_cr3(process->PD);
    currentProcess = process;

    create_thread(elf_load(elf));
}

void create_thread(const void* entry)
{
    Thread* thread = &TCBs[next_tid];
    uint8_t* stack = stacks + (next_tid * PAGE_SIZE) - 4;

    map(thread, NULL, PAGE_WRITE | PAGE_GLOBAL);
    map(stack,  NULL, PAGE_WRITE | PAGE_USER);

    thread->tid     = next_tid++;
    thread->process = currentProcess;
    list_append(&currentProcess->threads, &thread->processLink);

    memset(&thread->state, 0, sizeof(State));
    thread->state.cs  = USER_CODE | USER_RPL;
    thread->state.ds  = USER_DATA | USER_RPL;
    thread->state.es  = USER_DATA | USER_RPL;
    thread->state.ss  = USER_DATA | USER_RPL;
    thread->state.eip = (uint32_t)entry;
    thread->state.esp = (uint32_t)stack;
    thread->state.eflags = 0x202;

    list_prepend(&activeQueue, &thread->queueLink);
}

static void schedule(void)
{
    currentThread = list_item(list_pop(&activeQueue), Thread, queueLink);
    list_append(&activeQueue, &currentThread->queueLink);

    currentProcess = currentThread->process;
    if (read_cr3() != currentProcess->PD)
        write_cr3(currentProcess->PD);

    set_kernel_stack(&currentThread->state + 1);
    set_state(&currentThread->state);
}

void scheduler_init(void)
{
    irq_register(0, schedule);
}
