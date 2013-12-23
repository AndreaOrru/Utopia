#pragma once
#include <stdint.h>

#define INTERRUPT_GATE  0x8E
#define SYSCALL_GATE    0xEE

typedef void (*IsrStub)(void);

void idt_init(void);
void idt_set_gate(uint8_t i, uint8_t flags, IsrStub offset);
