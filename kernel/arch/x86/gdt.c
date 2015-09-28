#include <stdint.h>
#include "arch/x86/gdt.h"

#define KERNEL  0x90
#define USER    0xF0
#define CODE    0x0A
#define DATA    0x02

#define TSS_ACCESS  0x89
#define TSS_DESC    0x28

#define PROTECTED  (1 << 2)
#define BLOCKS_4K  (1 << 3)

typedef struct
{
    uint16_t limit;
    void* base;
} __attribute__((packed)) GDTR;

typedef struct
{
    unsigned limit_low  : 16;
    unsigned base_low   : 16;
    unsigned base_mid   : 8;
    unsigned access     : 8;
    unsigned limit_high : 4;
    unsigned flags      : 4;
    unsigned base_high  : 8;
} __attribute__((packed)) GDTEntry;

typedef struct
{
    uint32_t unused1;
    uint32_t esp0;
    uint32_t ss0;
    uint32_t unused2[22];
    uint16_t unused3;
    uint16_t iomap_base;
} __attribute__((packed)) TSS;

static GDTEntry gdt[] =
{
    {      0, 0, 0,             0,   0,                     0, 0 },
    { 0xFFFF, 0, 0, KERNEL | CODE, 0xF, PROTECTED | BLOCKS_4K, 0 },
    { 0xFFFF, 0, 0, KERNEL | DATA, 0xF, PROTECTED | BLOCKS_4K, 0 },
    { 0xFFFF, 0, 0,   USER | CODE, 0xF, PROTECTED | BLOCKS_4K, 0 },
    { 0xFFFF, 0, 0,   USER | DATA, 0xF, PROTECTED | BLOCKS_4K, 0 },
    {      0, 0, 0,    TSS_ACCESS,   0, PROTECTED            , 0 },
};

static GDTR gdtr = { sizeof(gdt), gdt };
static TSS tss;

extern void gdt_load(GDTR* gdtr);
extern void tss_load(uint8_t segment);

static void tss_init(void)
{
    uintptr_t base = (uintptr_t) &tss;
    uint32_t limit = sizeof(tss) - 1;
    uint16_t  desc = TSS_DESC >> 3;

    gdt[desc].base_low  =  base        & 0xFFFF;
    gdt[desc].base_mid  = (base >> 16) & 0xFF;
    gdt[desc].base_high = (base >> 24) & 0xFF;

    gdt[desc].limit_low  =  limit        & 0xFFFF;
    gdt[desc].limit_high = (limit >> 16) & 0xF;

    tss.ss0 = KERNEL_DATA;
    tss.iomap_base = sizeof(tss);
}

void gdt_init(void)
{
    tss_init();

    gdt_load(&gdtr);
    tss_load(TSS_DESC);
}
