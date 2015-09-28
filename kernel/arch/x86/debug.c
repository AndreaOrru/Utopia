#include <stdarg.h>
#include <stdio.h>
#include "debug.h"

void error(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    printf(format, args);
}
