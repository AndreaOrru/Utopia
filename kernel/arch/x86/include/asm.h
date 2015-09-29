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

extern void paging_enable(void* pd);

static inline void invlpg(void* v_addr)
{
    asm volatile ("invlpg (%0)" :: "r" (v_addr) : "memory");
}

static inline void* cr2_read(void)
{
    void* cr2;
    asm volatile ("mov %%cr2, %0" : "=r" (cr2));
    return cr2;
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
