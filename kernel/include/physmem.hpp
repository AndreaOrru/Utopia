#pragma once
#include "multiboot.h"

namespace PhysMem {

void  init(multiboot_info_t* info);
void* alloc();
void  free(void* address);

}
