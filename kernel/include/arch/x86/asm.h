#pragma once
#include <stdint.h>  // uint*_t.

static inline void hlt(void)
{
    asm volatile ("hlt");
}

static inline void cli(void)
{
    asm volatile ("cli");
}

static inline void sti(void)
{
    asm volatile ("sti");
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t val;
    asm volatile ("inb %1, %0" : "=a" (val) : "Nd" (port));
    return val;
}

static inline uint16_t inw(uint16_t port)
{
    uint16_t val;
    asm volatile ("inw %1, %0" : "=a" (val) : "Nd" (port));
    return val;
}

static inline void outb(uint16_t port, uint8_t val)
{
    asm volatile ("outb %0, %1" : : "a" (val), "Nd" (port));
}

static inline void outw(uint16_t port, uint16_t val)
{
    asm volatile ("outw %0, %1" : : "a" (val), "Nd" (port));
}
