#include <stddef.h>
#include "tty.h"

void tty_puts(const char* str)
{
    for (size_t i = 0; str[i] != '\0'; i++)
        tty_putchar(str[i]);
}
