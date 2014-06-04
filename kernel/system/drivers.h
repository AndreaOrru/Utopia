#pragma once
#include <stdint.h>

void irq_subscribe(uint8_t irq);
void irq_wait(uint8_t irq);
