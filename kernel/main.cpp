#include "gdt.hpp"
#include "heap.hpp"
#include "idt.hpp"
#include "pmem.hpp"
#include "scheduler.hpp"
#include "term.hpp"
#include "timer.hpp"
#include "vmem.hpp"
#include "x86.hpp"

using namespace Term;

extern "C" void main(multiboot_info_t* info)
{
    clear();

    GDT::init();
    IDT::init();
    PMem::init(info);
    VMem::init();
    Heap::init();
    Timer::init(100);
    Scheduler::init();

    printf("Hello world!");

    while (true);
}
