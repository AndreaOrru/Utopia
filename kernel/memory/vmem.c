#include "interrupt.h"
#include "layout.h"
#include "string.h"
#include "pmem.h"
#include "term.h"
#include "vmem.h"

typedef uintptr_t PEntry;

static PEntry* const PD  = (PEntry*)0xFFFFF000;
static PEntry* const PTs = (PEntry*)0xFFC00000;

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
        *pdEntry = (PEntry)frame_alloc() | flags | PAGE_PRESENT | PAGE_WRITE | PAGE_USER;
        invlpg(ptEntry);

        memset(PAGE_BASE(ptEntry), 0, PAGE_SIZE);
    }

    if (pAddr == NULL)
    {
        if (!(*ptEntry & PAGE_ALLOCATED))
            *ptEntry = (PEntry)frame_alloc() | flags | PAGE_PRESENT | PAGE_ALLOCATED;
        else
            *ptEntry = (*ptEntry & ~0xFFF)   | flags | PAGE_PRESENT | PAGE_ALLOCATED;
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
    Context* context = get_context();

    ALERT("Page fault at address %x.", read_cr2());
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

    hlt();
}

void* new_address_space(void)
{
    PEntry* extPD = (PEntry*)TMP_MAP;
    PEntry* newPD = frame_alloc();

    map(extPD, newPD, PAGE_WRITE | PAGE_GLOBAL);
    memset(extPD, 0, PAGE_SIZE);

    unsigned kSpace = PD_INDEX(KERNEL_SPACE_END);
    memcpy(extPD, PD, kSpace * sizeof(PEntry));

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
