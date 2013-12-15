#pragma once
#include "multiboot.h"

namespace PMem {

void  init(multiboot_info_t* info);
void* kernel_end();
void* alloc();
void  free(void* address);

}
