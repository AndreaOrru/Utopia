#pragma once
#include <stddef.h>

namespace Heap
{

void  init(size_t size);
void* alloc(size_t size);
void  free(void* addr);

}
