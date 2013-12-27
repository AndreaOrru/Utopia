#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>

#include <errno.h>
#undef errno
extern int errno;

char* __env[1] = { 0 };
char** environ = __env;

void _exit(int rc)
{
    while (1);
}

int close(int fd)
{
    return -1;
}

int execve(const char* name, char* const argv[], char* const env[])
{
    errno = ENOMEM;
    return -1;
}

int fork(void)
{
    errno = EAGAIN;
    return -1;
}

int fstat(int fd, struct stat* pstat)
{
    pstat->st_mode = S_IFCHR;
    return 0;
}

int getpid(void)
{
    return 1;
}

int isatty(int fd)
{
    return 1;
}

int kill(int pid, int sig)
{
    errno = EINVAL;
    return -1;
}

int link(const char* old, const char* new)
{
    errno = EMLINK;
    return -1;
}

off_t lseek(int fd, off_t pos, int whence)
{
    return 0;
}

int open(const char* file, int flags, ...)
{
    return -1;
}

size_t read(int fd, void* buf, size_t cnt)
{
    return 0;
}

void* sbrk(ptrdiff_t incr)
{
    errno = ENOMEM;
    return -1;
}

int stat(const char* file, struct stat* pstat)
{
    pstat->st_mode = S_IFCHR;
    return 0;
}

clock_t times(struct tms* buf)
{
    return -1;
}

int unlink(const char* file)
{
    errno = ENOENT;
    return -1;
}

pid_t wait(int* status)
{
    errno = ECHILD;
    return -1;
}

size_t write(int fd, const void* buf, size_t cnt)
{
    errno = ENOSYS;
    return -1;
}

// chown
// fcntl
// gettimeofday
// mkdir
// readlink
// rename
// symlink
