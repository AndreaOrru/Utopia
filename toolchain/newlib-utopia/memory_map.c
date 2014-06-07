#include <malloc.h>
#include <utopia.h>

void* memory_map(void* pAddr, size_t size, bool writable)
{
    void* vAddr = memalign(4096, size);

    _memory_map(vAddr, pAddr, size, writable);

    return vAddr;
}
