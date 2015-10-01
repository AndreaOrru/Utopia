#include <arch.h>       // PAGE_SIZE.
#include <asm.h>        // cr2_read, cr3_read, cr3_write, paging_enable.
#include <interrupt.h>  // context_get, interrupt_register.
#include <layout.h>     // KERNEL_SPACE_END, USER_SPACE.
#include <stdint.h>     // uint*_t.
#include <string.h>     // memset.
#include "pmem.h"       // frame_alloc.
#include "thread.h"     // thread_exit.
#include "tty.h"        // ALERT.
#include "vmem.h"

typedef uintptr_t PEntry;

static PEntry* const PD  = (PEntry*)0xFFFFF000;
static PEntry* const PTs = (PEntry*)0xFFC00000;

#define PD_INDEX(x) ((uintptr_t)(x) >> 22)
#define PT_INDEX(x) ((uintptr_t)(x) >> 12 & 0x3FF)

#define PD_ENTRY(x) (PD  +  PD_INDEX(x))
#define PT_ENTRY(x) (PTs + (PD_INDEX(x) * 0x400) + PT_INDEX(x))

#define X86_PRESENT    (1 << 0)
#define X86_WRITE      (1 << 1)
#define X86_USER       (1 << 2)
#define X86_4MB        (1 << 7)
#define X86_GLOBAL     (1 << 8)
#define X86_ALLOCATED  (1 << 9)

void* map(void* v_addr, void* p_addr, unsigned flags)
{
    if (v_addr == NULL)
        v_addr = (void*) TMP_MAP;

    unsigned x86_flags  = (flags & PAGE_WRITE)  ? X86_WRITE  : 0;
             x86_flags |= (flags & PAGE_USER)   ? X86_USER   : 0;
             x86_flags |= (flags & PAGE_GLOBAL) ? X86_GLOBAL : 0;

    PEntry* pd_entry = PD_ENTRY(v_addr);
    PEntry* pt_entry = PT_ENTRY(v_addr);

    if (! *pd_entry)
    {
        *pd_entry = (PEntry) frame_alloc() | x86_flags | X86_PRESENT | X86_WRITE | X86_USER;
        invlpg(pt_entry);

        memset(PAGE_BASE(pt_entry), 0, PAGE_SIZE);
    }

    if (p_addr == NULL)
    {
        if (*pt_entry & X86_ALLOCATED)
            *pt_entry = (*pt_entry & ~0xFFF)   | x86_flags | X86_PRESENT | X86_ALLOCATED;
        else
            *pt_entry = (PEntry) frame_alloc() | x86_flags | X86_PRESENT | X86_ALLOCATED;
    }
    else
    {
        if (*pt_entry & X86_ALLOCATED)
            frame_free((void*) *pt_entry);

        *pt_entry = (PEntry) PAGE_BASE(p_addr) | x86_flags | X86_PRESENT;
    }

    invlpg(v_addr);
    return v_addr;
}

void unmap(void* v_addr)
{
    PEntry* pd_entry = PD_ENTRY(v_addr);
    if (! *pd_entry)
        return;
    PEntry* pt_entry = PT_ENTRY(v_addr);

    if (*pt_entry & X86_ALLOCATED)
        frame_free((void*) *pt_entry);

    *pt_entry = 0;
    invlpg(v_addr);
}

void* addrspace_create(void)
{
    PEntry* phys_pd = frame_alloc();
    PEntry* virt_pd = map(NULL, phys_pd, PAGE_WRITE | PAGE_GLOBAL);
    memset(virt_pd, 0, PAGE_SIZE);

    unsigned kspace_entries = PD_INDEX(KERNEL_SPACE_END);
    memcpy(virt_pd, PD, kspace_entries * sizeof(PEntry));
    virt_pd[1023] = (PEntry) phys_pd | X86_PRESENT | X86_WRITE;

    return phys_pd;
}

void addrspace_destroy(void)
{
    PEntry* phys_pd = cr3_read();
    frame_free(phys_pd);

    for (unsigned i = PD_INDEX(USER_SPACE);
                  i < PD_INDEX(USER_SPACE_END);
                  i++)
    {
        if (PD[i])
        {
            frame_free((void*) (PD[i] & ~0xFFF));

            for (PEntry* pt_entry =  PT_ENTRY(0x400000*i);
                         pt_entry < (PT_ENTRY(0x400000*i) + 0x400);
                         pt_entry++)
            {
                if (*pt_entry & X86_ALLOCATED)
                    frame_free((void*) *pt_entry);
                *pt_entry = 0;
            }
        }
    }
}

void addrspace_switch(void* addrspace)
{
    cr3_write(addrspace);
}

void* addrspace_current(void)
{
    return cr3_read();
}

static void page_fault(void)
{
    void* cr2 = cr2_read();

    if (cr2 == (void*) THREAD_EXIT)
        return thread_exit(NULL);

    Context* context = context_get();

    ALERT("Page fault at address %x.", cr2);
    if (context->error & X86_USER)
        ALERT("  DPL: userspace  ");
    else
        ALERT("  DPL: kernel     ");

    if (context->error & X86_WRITE)
        ALERT("  OP:  write      ");
    else
        ALERT("  OP:  read       ");

    if (context->error & X86_PRESENT)
        ALERT("  ERR: protection ");
    else
        ALERT("  ERR: non-present");

    abort();
}

void vmem_init(void)
{
    PEntry* phys_pd = frame_alloc();
    memset(phys_pd, 0, PAGE_SIZE);

    phys_pd[0]    = (PEntry) 0x000000 | X86_PRESENT | X86_WRITE | X86_4MB | X86_GLOBAL;
    phys_pd[1023] = (PEntry) phys_pd  | X86_PRESENT | X86_WRITE;

    interrupt_register(14, page_fault);
    paging_enable(phys_pd);
}
