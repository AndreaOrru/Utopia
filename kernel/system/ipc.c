#include "layout.h"
#include "scheduler.h"
#include "string.h"
#include "ipc.h"

static TLS* const kernelTLSs = (TLS*)KERNEL_TLS;

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
    MsgBox*   senderBox = &kernelTLSs[  sender->tid].box;
    MsgBox* receiverBox = &kernelTLSs[receiver->tid].box;

    receiverBox->tag.n = senderBox->tag.n;
    memcpy(receiverBox->reg, senderBox->reg, senderBox->tag.n * sizeof(uint32_t));
}

void send_receive(uint16_t to, uint16_t from)
{
    Thread *sender, *receiver, *current = scheduler_current();

    if (to)
    {
        receiver = thread_get(to);

        if (receiver->state != WAIT_RECEIVING)
            return wait(current, to, WAIT_SENDING);

        if (!(receiver->listeningTo == (uint16_t)-1 || receiver->listeningTo == current->tid))
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
