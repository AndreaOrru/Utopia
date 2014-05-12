#pragma once
#include "thread.h"

void scheduler_init(void);
void scheduler_add(Thread* thread);
void scheduler_remove(Thread* thread);
Thread* scheduler_current(void);
void scheduler_wait(uint16_t waitFor, State reason);
void scheduler_unblock(Thread* thread);
