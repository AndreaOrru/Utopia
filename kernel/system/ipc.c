#include "layout.h"
#include "scheduler.h"
#include "string.h"
#include "ipc.h"

static UTCB* const kernelUTCBs = (UTCB*)KERNEL_UTCB;

static void wait(Thread* waiter, uint16_t tid, State reason)
{
    scheduler_remove(waiter);

    if (reason == WAIT_SENDING)
    {
        waiter->state = WAIT_SENDING;
        list_append(&thread_get(tid)->waitingList, &waiter->queueLink);
    }

    else if (reason == WAIT_RECEIVING)
    {
        waiter->state = WAIT_RECEIVING;
        waiter->listeningTo = tid;
    }
}

static alwaysinline void unblock(Thread* thread)
{
    thread->state = READY;
    scheduler_add(thread);
}

static inline void deliver(Thread* sender, Thread* receiver)
{
    VRegs*   senderBox = &kernelUTCBs[  sender->tid].vRegs;
    VRegs* receiverBox = &kernelUTCBs[receiver->tid].vRegs;

    receiverBox->tag.n = senderBox->tag.n;
    memcpy(receiverBox->regs, senderBox->regs, senderBox->tag.n * sizeof(uint32_t));
}

void send_receive(uint16_t to, uint16_t from)
{
    Thread *sender, *receiver, *current = scheduler_current();

    if (to)
    {
        receiver = thread_get(to);

        if (receiver->state != WAIT_RECEIVING)
            return wait(current, to, WAIT_SENDING);

        if (!(receiver->listeningTo == EVERYONE || receiver->listeningTo == current->tid))
            return;

        deliver(current, receiver);
        unblock(receiver);
    }

    if (from)
    {
        if (!list_empty(&current->waitingList))
        {
            sender = list_item(list_pop(&current->waitingList), Thread, queueLink);
            deliver(sender, current);
            unblock(sender);
        }
        else
            wait(current, from, WAIT_RECEIVING);
    }

    return;
}
