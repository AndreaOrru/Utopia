#include <sys/stat.h>

int fstat(int fd, struct stat* pstat)
{
    pstat->st_mode = S_IFCHR;
    return 0;
}
