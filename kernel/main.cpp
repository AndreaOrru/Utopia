#include "gdt.hpp"
#include "idt.hpp"
#include "irqs.hpp"
#include "machine.hpp"
#include "term.hpp"

extern "C" void main(void)
{
    GDT::init();
    IDT::init();

    Term::clear();
    Term::printf("Hello world!\n");

    while (true);
}
