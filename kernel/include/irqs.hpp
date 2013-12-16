#pragma once
#include "isrs.hpp"

namespace IRQs {

void init();
void register_handler(uint8_t n, IsrHandler handler, bool unmask = true);
void mask(uint8_t irq);
void unmask(uint8_t irq);

}
