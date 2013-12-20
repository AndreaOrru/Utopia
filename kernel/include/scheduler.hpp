#pragma once
#include "isrs.hpp"

namespace Scheduler {

struct Thread
{
    InterruptStack state;
    uint32_t tid;
};

void init();
void create_thread(void* entry);

}
