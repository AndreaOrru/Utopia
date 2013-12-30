#include <errno.h>
#undef errno
extern int errno;

int unlink(const char* file)
{
    errno = ENOENT;
    return -1;
}
