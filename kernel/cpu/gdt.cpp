#include "x86.hpp"
#include "gdt.hpp"

namespace GDT {

struct Tss
{
    uint32_t unused1;
    uint32_t esp0;
    uint32_t ss0;
    uint32_t unused2[22];
    uint16_t unused3;
    uint16_t iomapBase;
} __attribute__ ((packed));

struct GdtEntry
{
    unsigned limitLow  : 16;
    unsigned baseLow   : 16;
    unsigned baseMid   : 8;
    unsigned access    : 8;
    unsigned limitHigh : 4;
    unsigned flags     : 4;
    unsigned baseHigh  : 8;
} __attribute__((packed));

const uint8_t KERNEL = 0x90;
const uint8_t   USER = 0xF0;
const uint8_t   CODE = 0x0A;
const uint8_t   DATA = 0x02;
const uint8_t    TSS = 0x89;

GdtEntry gdt[] =
{
    {      0, 0, 0,             0,   0,   0, 0 },
    { 0xFFFF, 0, 0, KERNEL | CODE, 0xF, 0xC, 0 },
    { 0xFFFF, 0, 0, KERNEL | DATA, 0xF, 0xC, 0 },
    { 0xFFFF, 0, 0,   USER | CODE, 0xF, 0xC, 0 },
    { 0xFFFF, 0, 0,   USER | DATA, 0xF, 0xC, 0 },
    {      0, 0, 0,           TSS,   0, 0x4, 0 },
};

Tss tss;

void init_tss()
{
    auto base  = (uintptr_t)&tss;
    auto limit = sizeof(tss) - 1;
    auto desc  = TSS_DESC >> 3;

    gdt[desc].baseLow  =  base & 0xFFFF;
    gdt[desc].baseMid  = (base >> 16) & 0xFF;
    gdt[desc].baseHigh = (base >> 24) & 0xFF;

    gdt[desc].limitLow  =  limit        & 0xFFFF;
    gdt[desc].limitHigh = (limit >> 16) & 0xF;

    tss.ss0 = KERNEL_DATA;
    tss.iomapBase = sizeof(tss);
}

void set_kernel_stack(uint32_t esp0)
{
    tss.esp0 = esp0;
}

void init()
{
    init_tss();

    load_gdt((uintptr_t)gdt, sizeof(gdt));
    load_tss(TSS_DESC);
}

}
