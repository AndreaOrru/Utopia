#include "interrupt.h"
#include "layout.h"
#include "string.h"
#include "pmem.h"
#include "term.h"
#include "x86.h"
#include "vmem.h"

typedef uintptr_t PEntry;

static PEntry* PD    = (PEntry*)0xFFFFF000;
static PEntry* PTs   = (PEntry*)0xFFC00000;
static PEntry* extPD = (PEntry*)0xFFBFF000;

#define PAGE_ALLOCATED  (1 << 9)

#define PD_INDEX(x) ((uintptr_t)(x) >> 22)
#define PT_INDEX(x) ((uintptr_t)(x) >> 12 & 0x3FF)

#define PD_ENTRY(x) (PD  +  PD_INDEX(x))
#define PT_ENTRY(x) (PTs + (PD_INDEX(x) * 0x400) + PT_INDEX(x))

void* vmem_to_phys(const void* vAddr)
{
    PEntry* pdEntry = PD_ENTRY(vAddr);
    if (! *pdEntry)
        return NULL;

    PEntry* ptEntry = PT_ENTRY(vAddr);
    if (! *ptEntry)
        return NULL;

    return (void*) ((*ptEntry & ~0xFFF) | ((uintptr_t)vAddr & 0xFFF));
}

void map(void* vAddr, void* pAddr, uint16_t flags)
{
    PEntry* pdEntry = PD_ENTRY(vAddr);
    PEntry* ptEntry = PT_ENTRY(vAddr);

    if (! *pdEntry)
    {
        *pdEntry = (PEntry)frame_alloc() | flags | PAGE_PRESENT | PAGE_WRITE;
        invlpg(ptEntry);

        memset(PAGE_BASE(ptEntry), 0, PAGE_SIZE);
    }

    if (pAddr == NULL)
    {
        if (!(*ptEntry & PAGE_ALLOCATED))
            *ptEntry = (PEntry)frame_alloc() | flags | PAGE_PRESENT | PAGE_ALLOCATED;
    }
    else
    {
        if (*ptEntry & PAGE_ALLOCATED)
            frame_free((void*) *ptEntry);

        *ptEntry = (PEntry)PAGE_BASE(pAddr) | flags | PAGE_PRESENT;
    }

    invlpg(vAddr);
}

void unmap(void* vAddr)
{
    PEntry* pdEntry = PD_ENTRY(vAddr);
    if (! *pdEntry)
        return;
    PEntry* ptEntry = PT_ENTRY(vAddr);

    if (*ptEntry & PAGE_ALLOCATED)
        frame_free((void*) *ptEntry);

    *ptEntry = 0;
    invlpg(vAddr);
}

static void page_fault(void)
{
    State* state = get_state();

    printf("\n>>> Page Fault at address %x.\n", read_cr2());

    if (state->error & PAGE_USER)
        printf(">>> An user process ");
    else
        printf(">>> The kernel ");

    if (state->error & PAGE_WRITE)
        printf("tried to write ");
    else
        printf("tried to read ");

    if (state->error & PAGE_PRESENT)
        printf("causing a protection fault.");
    else
        printf("a non-present page.");

    hlt();
}

void* new_address_space(void)
{
    PEntry* newPD = frame_alloc();
    map(extPD, newPD, PAGE_WRITE);

    memset(extPD, 0, PAGE_SIZE);

    unsigned lowKernel = PD_INDEX(LOW_KERNEL);
    extPD[lowKernel] = PD[lowKernel];

    unsigned highKernel = PD_INDEX(HIGH_KERNEL);
    memcpy(&extPD[highKernel], &PD[highKernel], (1024 - highKernel) * sizeof(PEntry));

    extPD[1023] = (PEntry)newPD | PAGE_PRESENT | PAGE_WRITE;

    return newPD;
}

void vmem_init(void)
{
    PEntry* physPD = frame_alloc();
    memset(physPD, 0, PAGE_SIZE);

    physPD[0]    = (PEntry)0x000000 | PAGE_PRESENT | PAGE_WRITE | PAGE_4MB | PAGE_GLOBAL;
    physPD[1023] = (PEntry)physPD   | PAGE_PRESENT | PAGE_WRITE;

    interrupt_register(14, page_fault);
    enable_paging(physPD);
}
