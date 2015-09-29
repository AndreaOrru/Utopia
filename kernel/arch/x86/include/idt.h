#pragma once
#include <stdint.h>  // uint*_t.

#define INTERRUPT_GATE  0x8E

typedef void (*ISRStub)(void);

void idt_init(void);
void idt_gate_set(uint8_t i, uint8_t flags, ISRStub offset);
