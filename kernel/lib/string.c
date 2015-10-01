#include <assert.h>  // assert.
#include <stddef.h>  // NULL.
#include <string.h>

void* memcpy(void* dest, const void* src, size_t n)
{
    const char* s = src;
          char* d = dest;

    while (n--)
        *d++ = *s++;

    return dest;
}

void* memset(void* s, int c, size_t n)
{
    char* p = s;

    while (n--)
        *p++ = (char) c;

    return s;
}

char* strcpy(char* dest, const char* src)
{
    assert(dest != NULL);
    assert(src != NULL);

    char* ret = dest;

    while ((*dest++ = *src++));

    return ret;
}
