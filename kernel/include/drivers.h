#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void irq_subscribe(uint8_t irq);
void irq_wait(uint8_t irq);
void memory_map(void* vAddr, void* pAddr, size_t size, bool writable);
