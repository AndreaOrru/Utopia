#pragma once
#include <stdbool.h>  // bool.

void vmem_init(void);

void   map(void* v_addr, void* p_addr, bool write, bool user, bool global);
void unmap(void* v_addr);
