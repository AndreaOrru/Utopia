#pragma once
#include <stdint.h>

namespace GDT {

const uint8_t KERNEL_CODE = 0x08;
const uint8_t KERNEL_DATA = 0x10;
const uint8_t   USER_CODE = 0x18;
const uint8_t   USER_DATA = 0x20;

void init();

}
