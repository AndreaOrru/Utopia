#include <stdint.h>

extern "C" void kmain()
{
    *(char*)(0xB8000) = 'A';

    while (true);
}
