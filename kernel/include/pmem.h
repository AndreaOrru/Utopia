#pragma once
#include "multiboot.h"

void  pmem_init(multiboot_info_t* info);
void* frame_alloc();
void  frame_free(void* address);
