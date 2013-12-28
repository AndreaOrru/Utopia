#pragma once
#include "thread.h"

void scheduler_init(void);
void scheduler_add(Thread* thread);
Thread* scheduler_current(void);
Thread* scheduler_pop(void);
