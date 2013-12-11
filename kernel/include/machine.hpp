#pragma once
#include <stdint.h>

extern "C"
{
    void gdt_load(uintptr_t base, uint16_t limit);
    void idt_load(uintptr_t base, uint16_t limit);
}

inline void outb(uint16_t port, uint8_t val)
{
    asm volatile ("outb %0, %1" : : "a" (val), "Nd" (port));
}

inline void outw(uint16_t port, uint16_t val)
{
    asm volatile ("outw %0, %1" : : "a" (val), "Nd" (port));
}
