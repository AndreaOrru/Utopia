#pragma once
#include <stdint.h>

#define PAGE_PRESENT  (1 << 0)
#define PAGE_WRITE    (1 << 1)
#define PAGE_USER     (1 << 2)
#define PAGE_4MB      (1 << 7)
#define PAGE_GLOBAL   (1 << 8)

void  vmem_init(void);
void* vmem_to_phys(const void* vAddr);
void  map(void* vAddr, void* pAddr, uint16_t flags);
void  unmap(void* vAddr);
void* new_address_space(void);
