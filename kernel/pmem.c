#include <assert.h>  // assert.
#include <stddef.h>  // NULL, size_t.
#include <stdint.h>  // uint*_t.
#include "tty.h"     // ERROR.
#include "pmem.h"

#define FRAME_STACK  0x200000
#define PAGE_SIZE    0x1000

static void** stack = (void**) FRAME_STACK;
static size_t free_memory = 0;

inline void* frame_alloc(void)
{
    if (free_memory > 0)
        return *(--stack);
    else
        ERROR("Out of memory.");
}

inline void frame_free(void* address)
{
    *(stack++) = address;

    free_memory += PAGE_SIZE;
}

void pmem_init(multiboot_info_t* info)
{
    assert(info != NULL);
    assert(info->flags & MULTIBOOT_INFO_MEMORY);
    assert(info->flags & MULTIBOOT_INFO_MEM_MAP);

    size_t    stack_size = ((info->mem_upper * 1024) / PAGE_SIZE) * sizeof(void*);
    uintptr_t stack_end  = FRAME_STACK + stack_size;

    uintptr_t map = info->mmap_addr;
    while (map < info->mmap_addr + info->mmap_length)
    {
        multiboot_memory_map_t* entry = (void*) map;

        uintptr_t start = entry->addr;
        uintptr_t   end = start + (size_t) entry->len;
        start = (start >= stack_end) ? start : stack_end;

        if (entry->type == MULTIBOOT_MEMORY_AVAILABLE)
            for (uintptr_t i = start; i < end; i += PAGE_SIZE)
                frame_free((void*) i);

        map += entry->size + sizeof(entry->size);
    }
}
