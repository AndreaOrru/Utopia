#include "gdt.h"
#include "ipc.h"
#include "layout.h"
#include "string.h"
#include "vmem.h"
#include "scheduler.h"

static Process* const PCBs = (Process*)PCB_START;
static Thread*  const TCBs =  (Thread*)TCB_START;
extern UTCB* const kernelUTCBs;
extern UTCB* const   userUTCBs;

static LIST(readyQueue);
Process* volatile currentProcess;
Thread*  volatile currentThread;
static uint16_t next_pid = 1;
static uint16_t next_tid = 1;

void create_process(ElfHeader* elf)
{
    Process* process = &PCBs[next_pid];
    map(process, NULL, PAGE_WRITE | PAGE_GLOBAL);

    process->pid = next_pid++;
    process->PD  = new_address_space();
    process->nextLocalTid = 1;
    list_init(&process->threads);

    write_cr3(process->PD);
    currentProcess = process;

    create_thread(elf_load(elf));
}

void create_thread(const void* entry)
{
    Thread* thread = &TCBs[next_tid];
    map(thread, NULL, PAGE_WRITE | PAGE_GLOBAL);

    thread->tid      = next_tid++;
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

    list_prepend(&readyQueue, &thread->queueLink);
}

static void schedule(void)
{
    currentThread = list_item(list_pop(&readyQueue), Thread, queueLink);
    list_append(&readyQueue, &currentThread->queueLink);

    currentProcess = currentThread->process;
    if (read_cr3() != currentProcess->PD)
        write_cr3(currentProcess->PD);

    set_kernel_stack(&currentThread->context + 1);
    set_context(&currentThread->context);
}

void scheduler_init(void)
{
    irq_register(0, schedule);
}
