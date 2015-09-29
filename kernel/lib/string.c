#include <assert.h>  // assert.
#include <stddef.h>  // NULL.
#include <string.h>

char* strcpy(char* dest, const char* src)
{
    assert(dest != NULL);
    assert(src != NULL);

    char* ret = dest;

    while ((*dest++ = *src++));

    return ret;
}
