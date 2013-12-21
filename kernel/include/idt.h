#pragma once
#include <stdint.h>

typedef void (*IsrStub)(void);

void idt_init(void);
void idt_set_gate(uint8_t i, IsrStub offset);
