#pragma once
#include "isr.h"

void exception_init();
void exception_register(uint8_t n, IsrHandler handler);
