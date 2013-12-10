#include "gdt.hpp"
#include "idt.hpp"

extern "C" void kmain()
{
    GDT::init();
    IDT::init();

    *(char*)(0xB8000) = 'A';

    while (true);
}
