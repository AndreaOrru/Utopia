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

#define PAGE_PRESENT    (1 << 0)
#define PAGE_WRITE      (1 << 1)
#define PAGE_USER       (1 << 2)
#define PAGE_4MB        (1 << 7)
#define PAGE_GLOBAL     (1 << 8)
#define PAGE_ALLOCATED  (1 << 9)

void map(void* v_addr, void* p_addr, bool write, bool user, bool global)
{
    uint16_t flags = (write*PAGE_WRITE) | (user*PAGE_USER) | (global*PAGE_GLOBAL);
    PEntry* pd_entry = PD_ENTRY(v_addr);
    PEntry* pt_entry = PT_ENTRY(v_addr);

    if (! *pd_entry)
    {
        *pd_entry = (PEntry) frame_alloc() | flags | PAGE_PRESENT | PAGE_WRITE | PAGE_USER;
        invlpg(pt_entry);

        memset(PAGE_BASE(pt_entry), 0, PAGE_SIZE);
    }

    if (p_addr == NULL)
    {
        if (*pt_entry & PAGE_ALLOCATED)
            *pt_entry = (*pt_entry & ~0xFFF)   | flags | PAGE_PRESENT | PAGE_ALLOCATED;
        else
            *pt_entry = (PEntry) frame_alloc() | flags | PAGE_PRESENT | PAGE_ALLOCATED;
    }
    else
    {
        if (*pt_entry & PAGE_ALLOCATED)
            frame_free((void*) *pt_entry);

        *pt_entry = (PEntry) PAGE_BASE(p_addr) | flags | PAGE_PRESENT;
    }

    invlpg(v_addr);
}

void unmap(void* v_addr)
{
    PEntry* pd_entry = PD_ENTRY(v_addr);
    if (! *pd_entry)
        return;
    PEntry* pt_entry = PT_ENTRY(v_addr);

    if (*pt_entry & PAGE_ALLOCATED)
        frame_free((void*) *pt_entry);

    *pt_entry = 0;
    invlpg(v_addr);
}

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
    PEntry* phys_pd = frame_alloc();
    memset(phys_pd, 0, PAGE_SIZE);

    phys_pd[0]    = (PEntry) 0x000000 | PAGE_PRESENT | PAGE_WRITE | PAGE_4MB | PAGE_GLOBAL;
    phys_pd[1023] = (PEntry) phys_pd  | PAGE_PRESENT | PAGE_WRITE;

    interrupt_register(14, page_fault);
    paging_enable(phys_pd);
}
