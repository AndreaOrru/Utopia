#include "gdt.hpp"
#include "idt.hpp"

extern "C" void kmain()
{
    GDT::init();
    IDT::init();

    while (true);
}
