#include "gdt.hpp"
#include "idt.hpp"
#include "physmem.hpp"
#include "term.hpp"

using namespace Term;

extern "C" void main(multiboot_info_t* info)
{
    GDT::init();
    IDT::init();

    clear();
    printf("Hello world!\n");
    PhysMem::init(info);
    printf("%x ", PhysMem::alloc());
    printf("%x ", PhysMem::alloc());

    while (true);
}
