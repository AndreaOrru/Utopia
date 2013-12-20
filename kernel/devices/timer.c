#include "x86.h"
#include "timer.h"

#define PIT_CMD   0x43
#define PIT_CH_0  0x40

void timer_set_frequency(uint32_t hz)
{
    uint32_t divisor = 1193182 / hz;
    outb(PIT_CMD, 0x36);
    outb(PIT_CH_0,  divisor       & 0xFF);
    outb(PIT_CH_0, (divisor >> 8) & 0xFF);
}

void timer_init(uint32_t hz)
{
    timer_set_frequency(hz);
}
