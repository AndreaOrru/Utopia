#include <sys/stat.h>

int stat(const char* file, struct stat* pstat)
{
    pstat->st_mode = S_IFCHR;
    return 0;
}
