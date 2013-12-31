#pragma once
#include "x86.h"

#define ALERT(message...) \
    printf("\n\e0415" message)

#define ERROR(message...) \
({                        \
    ALERT(message);       \
    hlt();                \
})

#define STEP(function, message...)     \
({                                     \
    printf("\eF09  >> \eF15" message); \
    function;                          \
    printf("\eF10 OK!\n");             \
})

void put(char c);
void printf(const char* format, ...);
void clear_screen(void);
