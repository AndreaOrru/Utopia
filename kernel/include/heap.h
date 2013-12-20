#pragma once
#include <stddef.h>

void  heap_init();
void* malloc(size_t size);
void  free(void* addr);
