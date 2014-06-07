#include "interrupt.h"
#include "scheduler.h"
#include "vmem.h"
#include "drivers.h"

static bool    irqPending[32] = { [0 ... 31] = false };
static uint16_t irqThread[32] = { [0 ... 31] = 0 };

static void irq_notify(void)
{
    Context* context = get_context();

    uint8_t irq = IRQ(context->num);
    if (!irqThread[irq]) return;

    Thread* receiver = thread_get(irqThread[irq]);
    if (receiver->state == WAIT_RECEIVING && receiver->waitingFor == 0
                                          && receiver->waitingIrq == irq)
    {
        irqPending[irq] = false;
        scheduler_unblock(receiver);
    }
    else
        irqPending[irq] = true;
}

void irq_subscribe(uint8_t irq)
{
    if (irq == 0) return;
    Thread* current = scheduler_current();

    if (!irqThread[irq])
    {
        irq_register(irq, irq_notify);
        irqThread[irq] = current->tid;
    }
}

void irq_wait(uint8_t irq)
{
    Thread* current = scheduler_current();
    if (irqThread[irq] != current->tid) return;

    if (irqPending[current->waitingIrq])
        irqPending[current->waitingIrq] = false;
    else
    {
        current->waitingIrq = irq;
        scheduler_wait(0, WAIT_RECEIVING);
    }
}

void memory_map(void* vAddr, void* pAddr, size_t size, bool writable)
{
    for (size_t i = 0; i < size; i += PAGE_SIZE)
        map(vAddr + i, pAddr + i, PAGE_USER | (writable ? PAGE_WRITE : 0));
}
