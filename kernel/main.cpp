#include "gdt.hpp"
#include "heap.hpp"
#include "idt.hpp"
#include "pmem.hpp"
#include "term.hpp"
#include "timer.hpp"
#include "vmem.hpp"
#include "x86.hpp"

using namespace Term;

extern "C" void main(multiboot_info_t* info)
{
    GDT::init();
    IDT::init();
    PMem::init(info);
    VMem::init();
    Heap::init();
    Timer::init(100);

    clear();
    printf("Hello world!\n");

    for (int i = 0; i < 25; i++)
        printf("%d\n", i);

    while (true);
}
