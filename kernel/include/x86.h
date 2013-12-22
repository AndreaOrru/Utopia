#pragma once
#include <stdint.h>
#include "gcc.h"

#define PAGE_SIZE      0x1000
#define PAGE_BASE(x)   ((void*) ((uintptr_t)(x) & -PAGE_SIZE))
#define PAGE_ALIGN(x)  ((void*)(((uintptr_t)(x) +  PAGE_SIZE - 1) & -PAGE_SIZE))

extern void gdt_load(void* base, uint16_t limit);
extern void idt_load(void* base, uint16_t limit);
extern void tss_load(uint8_t segment);
extern void enable_paging(void* pd);

static alwaysinline void hlt(void)
{
    asm volatile ("hlt");
}

static alwaysinline void cli(void)
{
    asm volatile ("cli");
}

static alwaysinline void sti(void)
{
    asm volatile ("sti");
}

static alwaysinline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a" (ret) : "Nd" (port) );
    return ret;
}

static alwaysinline uint16_t inw(uint16_t port)
{
    uint16_t ret;
    asm volatile ("inb %1, %0" : "=a" (ret) : "Nd" (port) );
    return ret;
}

static alwaysinline void outb(uint16_t port, uint8_t val)
{
    asm volatile ("outb %0, %1" : : "a" (val), "Nd" (port));
}

static alwaysinline void outw(uint16_t port, uint16_t val)
{
    asm volatile ("outw %0, %1" : : "a" (val), "Nd" (port));
}

static alwaysinline void invlpg(void* vAddr)
{
    asm volatile ("invlpg (%0)" : : "r" (vAddr) : "memory");
}

static alwaysinline void* read_cr2(void)
{
    void* ret;
    asm volatile ("mov %%cr2, %0" : "=r" (ret));
    return ret;
}

static alwaysinline void* read_cr3(void)
{
    void* ret;
    asm volatile ("mov %%cr3, %0" : "=r" (ret));
    return ret;
}

static alwaysinline void write_cr3(void* cr3)
{
    asm volatile ("mov %0, %%cr3" : : "r" (cr3));
}
