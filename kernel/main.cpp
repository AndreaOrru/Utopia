#include "gdt.hpp"
#include "idt.hpp"
#include "term.hpp"

using namespace Term;

extern "C" void main(void)
{
    GDT::init();
    IDT::init();

    clear();
    printf("Hello world!\n");

    while (true);
}
