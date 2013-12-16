#pragma once
#include <stddef.h>

namespace Heap {

void  init();
void* alloc(size_t size);
void  free(void* addr);

}
