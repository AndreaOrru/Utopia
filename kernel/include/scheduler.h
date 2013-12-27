#pragma once
#include "thread.h"

void scheduler_init(void);
void switch_to(Thread* thread);
