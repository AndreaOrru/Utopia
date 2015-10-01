#pragma once
#include <stdbool.h>  // bool.

#define PAGE_WRITE   (1 << 0)
#define PAGE_USER    (1 << 1)
#define PAGE_GLOBAL  (1 << 2)

void vmem_init(void);

void*  map(void* v_addr, void* p_addr, unsigned flags);
void unmap(void* v_addr);

void* addrspace_create (void);
void  addrspace_destroy(void);
void  addrspace_switch (void* addrspace);
void* addrspace_current(void);
