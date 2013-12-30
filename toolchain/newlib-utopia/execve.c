#include <errno.h>
#undef errno
extern int errno;

int execve(const char* name, char* const argv[], char* const env[])
{
    errno = ENOMEM;
    return -1;
}
