#pragma once
#include <stdint.h>  // uintptr_t.

#define PAGE_SIZE      0x1000
#define PAGE_BASE(x)   ((void*) ((uintptr_t)(x) & -PAGE_SIZE))
#define PAGE_ALIGN(x)  ((void*)(((uintptr_t)(x) +  PAGE_SIZE - 1) & -PAGE_SIZE))

void arch_init(void);
