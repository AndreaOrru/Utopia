#include "string.h"
#include "pmem.hpp"
#include "x86.hpp"
#include "vmem.hpp"

namespace VMem {

typedef uintptr_t PEntry;

const uint16_t PRESENT   = (1 << 0);
const uint16_t WRITABLE  = (1 << 1);
const uint16_t SIZE_4MB  = (1 << 7);
const uint16_t ALLOCATED = (1 << 9);

auto const PD  = *(PEntry(*)[1024])      0xFFFFF000;
auto const PTs = *(PEntry(*)[1024][1024])0xFFC00000;

constexpr PEntry* PD_ENTRY(void* vAddr)
{
    return &PD[(uintptr_t)vAddr >> 22];
}

constexpr PEntry* PT_ENTRY(void* vAddr)
{
    return &PTs[(uintptr_t)vAddr >> 22][(uintptr_t)vAddr >> 12 & 0x3FF];
}

void* to_physical(void* vAddr)
{
    auto pdEntry = PD_ENTRY(vAddr);
    if (not *pdEntry)
        return NULL;

    auto ptEntry = PT_ENTRY(vAddr);
    if (not *ptEntry)
        return NULL;

    return (void*) ((*ptEntry & ~0xFFF) | ((uintptr_t)vAddr & 0xFFF));
}

void map(void* vAddr, void* pAddr, uint16_t flags)
{
    auto pdEntry = PD_ENTRY(vAddr);
    auto ptEntry = PT_ENTRY(vAddr);

    if (not *pdEntry)
    {
        *pdEntry = (PEntry)PMem::alloc() | flags | PRESENT | WRITABLE;
        invlpg((uintptr_t)ptEntry);

        memset(ptEntry, 0, PAGE_SIZE);
    }

    if (pAddr == NULL)
    {
        if (not (*ptEntry & ALLOCATED))
            *ptEntry = (PEntry)PMem::alloc() | flags | PRESENT | ALLOCATED;
    }
    else
    {
        if (*ptEntry & ALLOCATED)
            PMem::free((void*) *ptEntry);

        *ptEntry = (PEntry)PAGE_BASE(pAddr) | flags | PRESENT;
    }

    invlpg((uintptr_t)vAddr);
}

void unmap(void* vAddr)
{
    auto pdEntry = PD_ENTRY(vAddr);
    if (not *pdEntry)
        return;
    auto ptEntry = PT_ENTRY(vAddr);

    if (*ptEntry & ALLOCATED)
        PMem::free((void*) *ptEntry);

    *ptEntry = 0;
    invlpg((uintptr_t)vAddr);
}

void init()
{
    auto physPD = (PEntry*)PMem::alloc();
    memset(physPD, 0, PAGE_SIZE);

    physPD[0]    = (PEntry)0x0000 | PRESENT | WRITABLE | SIZE_4MB;
    physPD[1023] = (PEntry)physPD | PRESENT | WRITABLE;

    enable_paging((uintptr_t)physPD);
}

}
