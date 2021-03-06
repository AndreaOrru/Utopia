#pragma once
#include <stdint.h>

#define IRQ(interrupt)  (interrupt - 32)

typedef struct
{
    union
    {
        uint32_t regs[8];
        struct { uint32_t edi, esi, ebp, _esp, ebx, edx, ecx, eax; };
    };
    uint32_t num, error;
    uint32_t eip, cs, eflags, esp, ss;
} __attribute__((packed)) Context;

typedef void (*InterruptHandler)(void);

void interrupt_init(void);
void interrupt_register(uint8_t n, InterruptHandler handler);

void irq_register(uint8_t n, InterruptHandler handler);
void irq_mask  (uint8_t irq);
void irq_unmask(uint8_t irq);

void     set_context(Context* newContext);
Context* get_context(void);
