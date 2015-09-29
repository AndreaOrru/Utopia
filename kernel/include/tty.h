#pragma once
#include <stdio.h>   // printf.
#include <stdlib.h>  // abort.

void tty_init(void);
void tty_clear(void);

void tty_putchar(char c);
void tty_puts(const char* str);

#define ALERT(message...) \
    printf("\n\e0415" message)

#define ERROR(message...) \
({                        \
    ALERT(message);       \
    abort();              \
})

#define STEP(call, message...)         \
({                                     \
    printf("\eF09  >> \eF15" message); \
    call;                              \
    printf("\eF10 OK!\n");             \
})
