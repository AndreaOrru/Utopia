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

int chown(const char* path, uid_t owner, gid_t group)
{
    errno = ENOSYS;
    return -1;
}

int close(int file)
{
    return -1;
}

int execve(char* name, char** argv, char** env)
{
    errno = ENOMEM;
    return -1;
}

int fork(void)
{
    errno = EAGAIN;
    return -1;
}

int fstat(int file, struct stat* st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

int getpid(void)
{
    return 1;
}

int gettimeofday(struct timeval* ptimeval, void* ptimezone)
{
    errno = ENOSYS;
    return -1;
}

int isatty(int file)
{
    return 1;
}

int kill(int pid, int sig)
{
    errno = EINVAL;
    return -1;
}

int link(char* existing, char* new)
{
    errno = EMLINK;
    return -1;
}

int lseek(int file, int ptr, int dir)
{
    return 0;
}

int open(char* file, int flags, int mode)
{
    return -1;
}

int read(int file, char* ptr, int len)
{
    return 0;
}

int readlink(const char* path, char* buf, size_t bufsize)
{
    errno = ENOSYS;
    return -1;
}

caddr_t sbrk(int incr)
{
    errno = ENOMEM;
    return -1;
}

int stat(const char* file, struct stat* st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

int symlink(const char* path1, const char* path2)
{
    errno = ENOSYS;
    return -1;
}

clock_t times(struct tms* buf)
{
    return -1;
}

int unlink(char* name)
{
    errno = ENOENT;
    return -1;
}

int wait(int* status)
{
    errno = ECHILD;
    return -1;
}

int write(int file, char* ptr, int len)
{
    errno = ENOSYS;
    return -1;
}
