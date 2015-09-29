#include <arch.h>       // PAGE_SIZE.
#include <asm.h>        // cr2_read, paging_enable.
#include <interrupt.h>  // context_get, interrupt_register.
#include <stdint.h>     // uint*_t.
#include <string.h>     // memset.
#include "pmem.h"       // frame_alloc.
#include "tty.h"        // ALERT.
#include <vmem.h>

typedef uintptr_t PEntry;

static PEntry* const PD  = (PEntry*)0xFFFFF000;
static PEntry* const PTs = (PEntry*)0xFFC00000;

#define PD_INDEX(x) ((uintptr_t)(x) >> 22)
#define PT_INDEX(x) ((uintptr_t)(x) >> 12 & 0x3FF)

#define PD_ENTRY(x) (PD  +  PD_INDEX(x))
#define PT_ENTRY(x) (PTs + (PD_INDEX(x) * 0x400) + PT_INDEX(x))

#define PAGE_PRESENT  (1 << 0)
#define PAGE_WRITE    (1 << 1)
#define PAGE_USER     (1 << 2)
#define PAGE_4MB      (1 << 7)
#define PAGE_GLOBAL   (1 << 8)

static void page_fault(void)
{
    void* cr2 = cr2_read();
    Context* context = context_get();

    ALERT("Page fault at address %x.", cr2);
    if (context->error & PAGE_USER)
        ALERT("  DPL: userspace  ");
    else
        ALERT("  DPL: kernel     ");

    if (context->error & PAGE_WRITE)
        ALERT("  OP:  write      ");
    else
        ALERT("  OP:  read       ");

    if (context->error & PAGE_PRESENT)
        ALERT("  ERR: protection ");
    else
        ALERT("  ERR: non-present");

    abort();
}

void vmem_init(void)
{
    PEntry* physPD = frame_alloc();
    memset(physPD, 0, PAGE_SIZE);

    physPD[0]    = (PEntry) 0x000000 | PAGE_PRESENT | PAGE_WRITE | PAGE_4MB | PAGE_GLOBAL;
    physPD[1023] = (PEntry) physPD   | PAGE_PRESENT | PAGE_WRITE;

    interrupt_register(14, page_fault);
    paging_enable(physPD);
}
