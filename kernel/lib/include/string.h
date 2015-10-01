#pragma once
#include <stddef.h>  // size_t.

void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* s, int c, size_t n);
char* strcpy(char* dest, const char* src);
