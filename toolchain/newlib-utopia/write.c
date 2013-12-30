#include <sys/types.h>
#include <errno.h>
#undef errno
extern int errno;

size_t write(int fd, const void* buf, size_t cnt)
{
    errno = ENOSYS;
    return -1;
}
