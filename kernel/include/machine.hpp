#pragma once
#include <stdint.h>

#define alwaysinline inline __attribute__((always_inline))

extern "C"
{
    void gdt_load(uintptr_t base, uint16_t limit);
    void idt_load(uintptr_t base, uint16_t limit);
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
