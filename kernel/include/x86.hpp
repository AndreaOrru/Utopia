#pragma once
#include <stdint.h>

#define alwaysinline inline __attribute__((always_inline))

const uint16_t PAGE_SIZE = 0x1000;

constexpr void* PAGE_BASE(const void* addr)
{
    return (void*) ((uintptr_t)addr & -PAGE_SIZE);
}

constexpr void* PAGE_ALIGN(const void* addr)
{
    return (void*) (((uintptr_t)addr + PAGE_SIZE - 1) & -PAGE_SIZE);
}

extern "C"
{
    void load_gdt(uintptr_t base, uint16_t limit);
    void load_idt(uintptr_t base, uint16_t limit);
    void enable_paging(uintptr_t pd);
}

alwaysinline void hlt()
{
    asm volatile ("hlt");
}

alwaysinline void cli()
{
    asm volatile ("cli");
}

alwaysinline void sti()
{
    asm volatile ("sti");
}

alwaysinline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a" (ret) : "Nd" (port) );
    return ret;
}

alwaysinline uint16_t inw(uint16_t port)
{
    uint16_t ret;
    asm volatile ("inb %1, %0" : "=a" (ret) : "Nd" (port) );
    return ret;
}

alwaysinline void outb(uint16_t port, uint8_t val)
{
    asm volatile ("outb %0, %1" : : "a" (val), "Nd" (port));
}

alwaysinline void outw(uint16_t port, uint16_t val)
{
    asm volatile ("outw %0, %1" : : "a" (val), "Nd" (port));
}

alwaysinline void invlpg(uintptr_t vAddr)
{
    asm volatile ("invlpg (%0)" : : "r" (vAddr) : "memory");
}

alwaysinline uintptr_t read_cr2()
{
    uintptr_t ret;
    asm volatile ("mov %%cr2, %0" : "=r" (ret));
    return ret;
}
