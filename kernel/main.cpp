#include "gdt.hpp"
#include "idt.hpp"
#include "pmem.hpp"
#include "term.hpp"

using namespace Term;

extern "C" void main(multiboot_info_t* info)
{
    GDT::init();
    IDT::init();
    PMem::init(info);

    clear();
    printf("Hello world!\n");

    while (true);
}
