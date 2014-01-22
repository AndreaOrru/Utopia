#include <sys/types.h>
#include <utopia.h>

#include <errno.h>
#undef errno
extern int errno;

void* sbrk(ptrdiff_t incr)
{
    return _sbrk(incr);
}
