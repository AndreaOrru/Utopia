#include "gdt.hpp"
#include "irqs.hpp"
#include "list.hpp"
#include "scheduler.hpp"

namespace Scheduler {

List<Thread*>* activeQueue;
extern "C" { Thread* currentThread = NULL; }

void create_thread(void* entry)
{
    static uint32_t tid = 0;

    auto thread = new Thread;
    thread->tid = tid++;

    auto user_stack = new uint8_t[0x1000];
    thread->state.eflags = 0x202;
    thread->state.eip = (uint32_t)entry;
    thread->state.esp = (uint32_t)user_stack + sizeof(user_stack) - 4;
    thread->state.cs  = GDT::USER_CODE | GDT::USER_RPL;
    thread->state.ss  = GDT::USER_DATA | GDT::USER_RPL;

    activeQueue->push_back(thread);
}

void schedule(InterruptStack*)
{
    if (currentThread)
        activeQueue->push_back(currentThread);
    currentThread = activeQueue->pop_front();

    GDT::set_kernel_stack((uint32_t)(&currentThread->state + 1));
}

void init()
{
    activeQueue = new List<Thread*>;

    IRQs::register_handler(0, schedule);
}

}
