#pragma once
#include "thread.h"  // Thread.

void scheduler_init(void);

void schedule_to(Thread* thread);
void scheduler_remove(Thread* thread);
Thread* scheduler_current(void);

void schedule(void);
