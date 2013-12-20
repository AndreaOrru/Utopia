#pragma once
#include <stdint.h>

typedef void (*IsrStub)();

void idt_init();
void idt_set_gate(uint8_t i, IsrStub offset);
