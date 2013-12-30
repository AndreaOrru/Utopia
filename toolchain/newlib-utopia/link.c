#include <errno.h>
#undef errno
extern int errno;

int link(const char* old, const char* new)
{
    errno = EMLINK;
    return -1;
}
