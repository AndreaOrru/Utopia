#include <stddef.h>
#include <stdint.h>
#include "x86.hpp"
#include "physmem.hpp"

namespace PhysMem {

extern "C" void* _end;
void** stack;
size_t stackSize;
size_t ram = 0;

void* alloc()
{
    return *(--stack);
}

void free(void* address)
{
    *(stack++) = PAGE_BASE(address);
}

void calculate_ram(multiboot_info_t* info)
{
    auto map = info->mmap_addr;
    while (map < info->mmap_addr + info->mmap_length)
    {
        auto entry = (multiboot_memory_map_t*)map;

        if (entry->type == MULTIBOOT_MEMORY_AVAILABLE)
            ram += entry->len;

        map += entry->size + sizeof(entry->size);
    }
}

void init_stack(multiboot_info_t* info)
{
    auto stackEnd = (uintptr_t)PAGE_ALIGN(stack + (ram / 0x1000) * sizeof(void*));

    auto map = info->mmap_addr;
    while (map < info->mmap_addr + info->mmap_length)
    {
        auto entry = (multiboot_memory_map_t*)map;

        uintptr_t start = entry->addr;
        uintptr_t   end = start + (size_t)entry->len;
        start = (start > stackEnd) ? start : stackEnd;

        if (entry->type == MULTIBOOT_MEMORY_AVAILABLE)
            for (auto i = start; i < end; i += 0x1000)
                free((void*)i);

        map += entry->size + sizeof(entry->size);
    }
}

void init(multiboot_info_t* info)
{
    calculate_ram(info);

    stack = (void**)PAGE_ALIGN(&_end);

    init_stack(info);
}

}
