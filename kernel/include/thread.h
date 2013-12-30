#pragma once
#include "interrupt.h"
#include "ipc.h"
#include "process.h"
#include "x86.h"

typedef struct
{
    VRegs vRegs;
    uint8_t tls[PAGE_SIZE - sizeof(VRegs)];
} UTCB;

typedef enum { NEW, READY, WAIT_SENDING, WAIT_RECEIVING } State;

typedef union
{
    struct
    {
        uint16_t tid;
        uint16_t localTid;
        Link queueLink;

        State state;
        uint16_t listeningTo;
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
Thread* thread_get(uint16_t tid);
