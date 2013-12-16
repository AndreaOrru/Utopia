#include <stddef.h>
#include "x86.hpp"
#include "pmem.hpp"

namespace PMem {

auto stack = (uint16_t*)0x200000;

void* alloc()
{
    return (void*) (*(--stack) * PAGE_SIZE);
}

void free(void* address)
{
    *(stack++) = (uintptr_t)address / PAGE_SIZE;
}

void init(multiboot_info_t* info)
{
    auto map = info->mmap_addr;
    while (map < info->mmap_addr + info->mmap_length)
    {
        auto entry = (multiboot_memory_map_t*)map;

        uintptr_t start = entry->addr;
        uintptr_t   end = start + (size_t)entry->len;
        start = (start >= 0x800000) ? start : 0x800000;

        if (entry->type == MULTIBOOT_MEMORY_AVAILABLE)
            for (auto i = start; i < end; i += PAGE_SIZE)
                free((void*)i);

        map += entry->size + sizeof(entry->size);
    }
}

}
