#include "gdt.hpp"
#include "idt.hpp"
#include "term.hpp"

extern "C" void main(void)
{
    GDT::init();
    IDT::init();

    Term::clear();
    Term::printf("Hello world!\n");

    int a = 5 / 0;

    while (true);
}
