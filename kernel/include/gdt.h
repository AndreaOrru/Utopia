#pragma once
#include <stdint.h>

#define    USER_RPL   0b11
#define KERNEL_CODE   0x08
#define KERNEL_DATA   0x10
#define   USER_CODE   0x18
#define   USER_DATA   0x20

void gdt_init(void);
void set_kernel_stack(uint32_t esp0);
