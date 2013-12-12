#pragma once
#include "isrs.hpp"

namespace Exceptions
{

void init();
void register_handler(uint8_t n, IsrHandler handler);

}
