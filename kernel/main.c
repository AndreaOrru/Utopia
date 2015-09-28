#include <stdio.h>
#include "arch.h"
#include "tty.h"

void main(void* multiboot __attribute__((unused)))
{
    tty_init();
    arch_init();

    printf("Hello world!");
}
