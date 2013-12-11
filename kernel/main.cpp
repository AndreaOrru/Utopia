#include "gdt.hpp"
#include "idt.hpp"
#include "text.hpp"

extern "C" void main(void)
{
    GDT::init();
    IDT::init();

    Text::clear();
    Text::printf("%d", -0x1000);

    while (true);
}
