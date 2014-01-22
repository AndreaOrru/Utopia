#include <sys/types.h>
#include <utopia.h>

#include <errno.h>
#undef errno
extern int errno;

size_t write(int fd, const void* buf, size_t cnt)
{
    int i;

    if (fd == 1)
    {
        for (i = 0; i < cnt; i++)
            put(((char*)buf)[i]);
        return cnt;
    }

    errno = ENOSYS;
    return -1;
}
