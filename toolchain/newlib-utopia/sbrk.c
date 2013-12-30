#include <sys/types.h>
#include <errno.h>
#undef errno
extern int errno;

void* sbrk(ptrdiff_t incr)
{
    errno = ENOMEM;
    return -1;
}
