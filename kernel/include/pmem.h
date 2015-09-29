#pragma once
#include "multiboot.h"  // multiboot_info_t.

void pmem_init(const multiboot_info_t* info);

void* frame_alloc(void);
void  frame_free (const void* address);
