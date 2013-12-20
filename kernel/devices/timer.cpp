#include "term.hpp"
#include "x86.hpp"
#include "timer.hpp"

namespace Timer {

const uint16_t PIT_CMD  = 0x43;
const uint16_t PIT_CH_0 = 0x40;

void set_frequency(uint32_t hz)
{
    uint32_t divisor = 1193182 / hz;
    outb(PIT_CMD, 0x36);
    outb(PIT_CH_0,  divisor       & 0xFF);
    outb(PIT_CH_0, (divisor >> 8) & 0xFF);
}

void init(uint32_t hz)
{
    set_frequency(hz);
}

}
