#pragma once
#include <stdint.h>

namespace VMem {

void  init();
void* to_physical(void* vAddr);
void  map(void* vAddr, void* pAddr, uint16_t flags);
void  unmap(void* vAddr);

}
