#pragma once
#include <stdio.h>

void hang(void);

#define ERROR(message...)       \
({                              \
    printf("\n\e0415" message); \
    hang();                     \
})
