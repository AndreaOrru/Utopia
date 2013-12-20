#pragma once
#include "isr.h"

void irq_init();
void irq_register(uint8_t n, IsrHandler handler);
void irq_mask(uint8_t irq);
void irq_unmask(uint8_t irq);
