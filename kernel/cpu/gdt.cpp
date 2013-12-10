#include "gdt.hpp"

namespace GDT
{

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

GdtEntry gdt[] =
{
    {      0, 0, 0,             0,   0,   0, 0 },
    { 0xFFFF, 0, 0, KERNEL | CODE, 0xF, 0xC, 0 },
    { 0xFFFF, 0, 0, KERNEL | DATA, 0xF, 0xC, 0 },
    { 0xFFFF, 0, 0,   USER | CODE, 0xF, 0xC, 0 },
    { 0xFFFF, 0, 0,   USER | DATA, 0xF, 0xC, 0 }
};

extern "C" void gdt_load(GdtEntry* base, uint16_t limit);

void init()
{
    gdt_load(gdt, sizeof(gdt));
}

}
