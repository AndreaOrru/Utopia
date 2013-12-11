#pragma once
#include <stdint.h>

namespace IDT
{

void init();
void set_gate(uint8_t i, void (*offset)(void));

}
