#include "pmem.hpp"
#include "heap.hpp"

namespace Heap
{

struct Header
{
    Header* next;
    size_t units;
};

Header* freeList;

void* alloc(size_t size)
{
    size_t units = (size + sizeof(Header) - 1) / sizeof(Header) + 1;
    auto prev = freeList;

    for (auto curr = prev->next; ; prev = curr, curr = curr->next)
        if (curr->units >= units)
        {
            if (curr->units == units)
                prev->next = curr->next;
            else
            {
                curr->units -= units;
                curr += curr->units;
                curr->units = units;
            }

            freeList = prev;
            return (void*)(curr + 1);
        }
}

void free(void* addr)
{
    auto block = (Header*)addr - 1;
    auto curr = freeList;

    while (not (block > curr && block < curr->next))
        if (curr >= curr->next && (block > curr || block < curr->next))
            break;
        else
            curr = curr->next;

    if (block + block->units == curr->next)
    {
        block->units += curr->next->units;
        block->next = curr->next->next;
    }
    else
        block->next = curr->next;

    if (curr + curr->units == block)
    {
        curr->units += block->units;
        curr->next = block->next;
    }
    else
        curr->next = block;

    freeList = curr;
}

void init(size_t size)
{
    freeList = (Header*)PMem::kernel_end();
    freeList->next  = freeList;
    freeList->units = (size + sizeof(Header) - 1) / sizeof(Header) + 1;
}

}
