#pragma once
#include <stdint.h>

namespace GDT {

const uint8_t    USER_RPL = 0b11;

const uint8_t KERNEL_CODE = 0x08;
const uint8_t KERNEL_DATA = 0x10;
const uint8_t   USER_CODE = 0x18;
const uint8_t   USER_DATA = 0x20;
const uint8_t    TSS_DESC = 0x28;

void init();
void set_kernel_stack(uint32_t esp0);

}
