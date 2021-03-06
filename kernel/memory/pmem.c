#include <stddef.h>
#include "layout.h"
#include "x86.h"
#include "pmem.h"

static uint16_t* stack = (uint16_t*)FRAME_STACK;

alwaysinline void* frame_alloc(void)
{
    return (void*) (*(--stack) * PAGE_SIZE);
}

alwaysinline void frame_free(void* address)
{
    *(stack++) = (uintptr_t)address / PAGE_SIZE;
}

void pmem_init(multiboot_info_t* info)
{
    uintptr_t map = info->mmap_addr;
    while (map < info->mmap_addr + info->mmap_length)
    {
        multiboot_memory_map_t* entry = (void*)map;

        uintptr_t start = entry->addr;
        uintptr_t   end = start + (size_t)entry->len;
        start = (start >= FRAME_STACK_END) ? start : FRAME_STACK_END;

        if (entry->type == MULTIBOOT_MEMORY_AVAILABLE)
            for (uintptr_t i = start; i < end; i += PAGE_SIZE)
                frame_free((void*)i);

        map += entry->size + sizeof(entry->size);
    }
}
