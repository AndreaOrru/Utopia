#include <stdint.h>
#include "gdt.hpp"

extern "C" void kmain()
{
    GDT::init();

    *(char*)(0xB8000) = 'A';

    while (true);
}
