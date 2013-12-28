#include "layout.h"
#include "scheduler.h"
#include "string.h"
#include "ipc.h"

static UTCB* const kernelUTCBs = (UTCB*)KERNEL_UTCB;

static inline void wait_for(uint16_t tid, State reason)
{
    Thread* current = scheduler_pop();

    if (reason == WAIT_SENDING)
    {
        current->state = WAIT_SENDING;
        list_append(&thread_get(tid)->waitingList, &current->queueLink);
    }

    else if (reason == WAIT_RECEIVING)
    {
        current->state = WAIT_RECEIVING;
        current->listeningTo = tid;
    }
}

static alwaysinline void unblock(Thread* thread)
{
    thread->state = READY;
    scheduler_add(thread);
}

static void deliver(Thread* sender, Thread* receiver)
{
    UTCB*   senderBox = &kernelUTCBs[  sender->tid];
    UTCB* receiverBox = &kernelUTCBs[receiver->tid];

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
            return wait_for(to, WAIT_SENDING);

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
            wait_for(from, WAIT_RECEIVING);
    }

    return;
}
