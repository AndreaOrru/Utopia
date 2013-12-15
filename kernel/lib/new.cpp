#include "heap.hpp"

void* operator new(size_t size)
{
    return Heap::alloc(size);
}

void* operator new[](size_t size)
{
    return Heap::alloc(size);
}

void operator delete(void* p)
{
    Heap::free(p);
}

void operator delete[](void* p)
{
    Heap::free(p);
}
