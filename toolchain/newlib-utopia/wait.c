#include <sys/types.h>
#include <errno.h>
#undef errno
extern int errno;

pid_t wait(int* status)
{
    errno = ECHILD;
    return -1;
}
