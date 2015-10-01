#include <asm.h>     // outb.
#include <assert.h>  // assert.
#include <stddef.h>  // NULL.
#include "timer.h"

#define PIT_CMD   0x43
#define PIT_CH_0  0x40
#define PIT_FREQ  1193182

#define SQUARE_WAVE_GEN  (0b011 << 1)
#define LSB_THEN_MSB     (0b11  << 4)

void timer_init(unsigned hz)
{
    assert(hz != 0);
    unsigned divisor = PIT_FREQ / hz;

    outb(PIT_CMD, SQUARE_WAVE_GEN | LSB_THEN_MSB);

    outb(PIT_CH_0,  divisor       & 0xFF);
    outb(PIT_CH_0, (divisor >> 8) & 0xFF);
}

void timer_handler_set(InterruptHandler handler)
{
    assert(handler != NULL);

    irq_register(0, handler);
}
