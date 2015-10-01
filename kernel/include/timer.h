#pragma once
#include <interrupt.h>  // InterruptHandler.

void timer_init(unsigned hz);
void timer_handler_set(InterruptHandler handler);
