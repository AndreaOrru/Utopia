#include "layout.h"
#include "scheduler.h"
#include "string.h"
#include "ipc.h"

static TLS* const kernelTLSs = (TLS*)KERNEL_TLS;

static inline void deliver(Thread* sender, Thread* receiver)
{
    MsgBox*   senderBox = &kernelTLSs[  sender->tid].box;
    MsgBox* receiverBox = &kernelTLSs[receiver->tid].box;

    receiverBox->tag.pid = sender->process->pid;
    receiverBox->tag.tid = sender->tid;
    receiverBox->tag.n   = senderBox->tag.n;

    memcpy(receiverBox->reg, senderBox->reg, senderBox->tag.n * sizeof(uint32_t));
}

void send_receive(uint16_t to, uint16_t from)
{
    Thread *sender, *receiver, *current = scheduler_current();

    if (to)
    {
        receiver = thread_get(to);

        if (receiver->state != WAIT_RECEIVING)
            return scheduler_wait(to, WAIT_SENDING);

        if (!(receiver->waitingFor == (uint16_t)-1 || receiver->waitingFor == current->tid))
            return;

        deliver(current, receiver);
        scheduler_unblock(receiver);
    }

    if (from)
    {
        if (!list_empty(&current->waitingList))
        {
            sender = list_item(list_pop(&current->waitingList), Thread, queueLink);
            deliver(sender, current);
            scheduler_unblock(sender);
        }
        else
            scheduler_wait(from, WAIT_RECEIVING);
    }

    return;
}
