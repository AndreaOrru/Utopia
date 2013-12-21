#include "x86.h"
#include "gdt.h"

#define KERNEL  0x90
#define USER    0xF0
#define CODE    0x0A
#define DATA    0x02

#define TSS       0x89
#define TSS_DESC  0x28

typedef struct
{
    uint32_t unused1;
    uint32_t esp0;
    uint32_t ss0;
    uint32_t unused2[22];
    uint16_t unused3;
    uint16_t iomapBase;
} __attribute__ ((packed)) Tss;

typedef struct
{
    unsigned limitLow  : 16;
    unsigned baseLow   : 16;
    unsigned baseMid   : 8;
    unsigned access    : 8;
    unsigned limitHigh : 4;
    unsigned flags     : 4;
    unsigned baseHigh  : 8;
} __attribute__((packed)) GdtEntry;

static GdtEntry gdt[] =
{
    {      0, 0, 0,             0,   0,   0, 0 },
    { 0xFFFF, 0, 0, KERNEL | CODE, 0xF, 0xC, 0 },
    { 0xFFFF, 0, 0, KERNEL | DATA, 0xF, 0xC, 0 },
    { 0xFFFF, 0, 0,   USER | CODE, 0xF, 0xC, 0 },
    { 0xFFFF, 0, 0,   USER | DATA, 0xF, 0xC, 0 },
    {      0, 0, 0,           TSS,   0, 0x4, 0 },
};

static Tss tss;

static void tss_init(void)
{
    uintptr_t base = (uintptr_t)&tss;
    uint32_t limit = sizeof(tss) - 1;
    uint16_t  desc = TSS_DESC >> 3;

    gdt[desc].baseLow  =  base & 0xFFFF;
    gdt[desc].baseMid  = (base >> 16) & 0xFF;
    gdt[desc].baseHigh = (base >> 24) & 0xFF;

    gdt[desc].limitLow  =  limit        & 0xFFFF;
    gdt[desc].limitHigh = (limit >> 16) & 0xF;

    tss.ss0 = KERNEL_DATA;
    tss.iomapBase = sizeof(tss);
}

void set_kernel_stack(void* esp0)
{
    tss.esp0 = (uint32_t)esp0;
}

void gdt_init(void)
{
    tss_init();

    gdt_load(gdt, sizeof(gdt));
    tss_load(TSS_DESC);
}
