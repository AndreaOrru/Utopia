#include "gdt.hpp"
#include "idt.hpp"
#include "pmem.hpp"
#include "vmem.hpp"
#include "term.hpp"

using namespace Term;

extern "C" void main(multiboot_info_t* info)
{
    GDT::init();
    IDT::init();
    PMem::init(info);
    VMem::init();

    clear();
    printf("Hello world!\n");

    while (true);
}
