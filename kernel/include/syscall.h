#pragma once
#include "interrupt.h"

void syscall_init(void);
void syscall_register(uint8_t n, InterruptHandler handler);
