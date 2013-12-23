#include "x86.h"
#include "timer.h"

#define PIT_CMD   0x43
#define PIT_CH_0  0x40

#define SQUARE_WAVE_GEN  (0b011 << 1)
#define LSB_THEN_MSB     (0b11  << 4)

void timer_init(uint32_t hz)
{
    uint32_t divisor = 1193182 / hz;

    outb(PIT_CMD, SQUARE_WAVE_GEN | LSB_THEN_MSB);
    outb(PIT_CH_0,  divisor       & 0xFF);
    outb(PIT_CH_0, (divisor >> 8) & 0xFF);
}
