#include <stdint.h>
#include "heap.h"

typedef struct header
{
    struct header* next;
    size_t units;
} Header;

static Header* freeList = (Header*)0x600000;

void* malloc(size_t size)
{
    size_t units = (size + sizeof(Header) - 1) / sizeof(Header) + 1;
    Header* prev = freeList;

    for (Header* curr = prev->next; ; prev = curr, curr = curr->next)
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
    Header* block = (Header*)addr - 1;
    Header* curr = freeList;

    while (!(block > curr && block < curr->next))
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

void heap_init()
{
    freeList->next  = freeList;
    freeList->units = 0x200000 / sizeof(Header);
}
