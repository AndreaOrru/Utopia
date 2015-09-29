#pragma once
#include <stdint.h>  // uint*_t.

#define IRQ(interrupt)  (interrupt - 32)

typedef struct
{
    union
    {
        uint32_t regs[8];
        struct { uint32_t edi, esi, ebp, _esp, ebx, edx, ecx, eax; };
    };
    uint32_t int_n, error;
    uint32_t eip, cs, eflags, esp, ss;
} __attribute__((packed)) Context;

typedef void (*InterruptHandler)(void);

void interrupt_init(void);
void interrupt_register(uint8_t n, InterruptHandler handler);

void irq_register(uint8_t irq, InterruptHandler handler);
void irq_mask    (uint8_t irq);
void irq_unmask  (uint8_t irq);

void     context_set(Context* new_context);
Context* context_get(void);
