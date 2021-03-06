#pragma once
#include "interrupt.h"
#include "ipc.h"
#include "process.h"
#include "x86.h"

#define THREAD_MAGIC  0xDEADC0DE

typedef struct
{
    MsgBox box;
    uint8_t mem[PAGE_SIZE - sizeof(MsgBox)];
} TLS;

typedef enum { READY, DYING, WAIT_SENDING, WAIT_RECEIVING, WAIT_SEND_RECV } State;

typedef union
{
    struct
    {
        uint16_t tid;
        uint16_t localTid;
        Link queueLink;

        State state;
        uint16_t waitingFor;
        uint8_t waitingIrq;
        List waitingList;

        Process* process;
        Link processLink;
    };
    struct
    {
        uint8_t kernelStack[PAGE_SIZE - sizeof(Context)];
        Context context;
    };
} Thread;

void thread_create(const void* entry, Process* process);
void thread_exit(Thread* thread);
Thread* thread_get(uint16_t tid);
