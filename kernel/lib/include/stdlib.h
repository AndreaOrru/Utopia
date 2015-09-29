#pragma once
#include <stddef.h>  // NULL.

void abort(void) __attribute__((noreturn));
char* itoa(unsigned val, char* buf, int base);
