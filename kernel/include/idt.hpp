#pragma once
#include <stdint.h>

namespace IDT {

typedef void (*IsrStub)();

void init();
void set_gate(uint8_t i, IsrStub offset);

}
