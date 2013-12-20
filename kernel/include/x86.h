#pragma once
#include <stdint.h>

#define alwaysinline inline __attribute__((always_inline))

#define PAGE_SIZE      0x1000
#define PAGE_BASE(x)   ((void*) ((uintptr_t)(x) & -PAGE_SIZE))
#define PAGE_ALIGN(x)  ((void*)(((uintptr_t)(x) +  PAGE_SIZE - 1) & -PAGE_SIZE))

extern void gdt_load(uintptr_t base, uint16_t limit);
extern void idt_load(uintptr_t base, uint16_t limit);
extern void tss_load(uint8_t segment);
extern void enable_paging(uintptr_t pd);

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
