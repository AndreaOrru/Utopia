#include <assert.h>  // assert.
#include <stddef.h>  // NULL, size_t.
#include "tty.h"

void tty_puts(const char* str)
{
    assert(str != NULL);

    for (size_t i = 0; str[i] != '\0'; i++)
        tty_putchar(str[i]);
}
