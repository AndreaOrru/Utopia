#include "string.h"

int memcmp(const void* s1, const void* s2, size_t n)
{
    const unsigned char *p1 = (const unsigned char*)s1;
    const unsigned char *p2 = (const unsigned char*)s2;

    for (; n--; p1++, p2++)
    {
	    unsigned char u1 = *p1, u2 = *p2;

	    if (u1 != u2)
	        return (u1 - u2);
    }

    return 0;
}

void* memcpy(void* dest, const void* src, size_t n)
{
    const char* sp = src;
          char* dp = dest;

    while (n--)
        *dp++ = *sp++;

    return dest;
}

void* memmove(void* dest, const void* src, size_t n)
{
    const unsigned char* ps = src;
          unsigned char* pd = dest;

    if (ps < pd)
        for (pd += n, ps += n; n--;)
            *--pd = *--ps;
    else
        while (n--)
            *pd++ = *ps++;

    return dest;
}

void* memset(void* s, int c, size_t n)
{
    unsigned char* p = s;

    while (n--)
        *p++ = (unsigned char)c;

    return s;
}

char* strcat(char* dest, const char* src)
{
    char* ret = dest;
    dest += strlen(dest);

    while ((*dest++ = *src++));

    return ret;
}

int strcmp(const char* s1, const char* s2)
{
    while (*s1 && (*s1 == *s2))
        s1++, s2++;

    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

char* strcpy(char* dest, const char* src)
{
    char* ret = dest;

    while ((*dest++ = *src++));

    return ret;
}

size_t strlen(const char* s)
{
    const char *p = s;

    while (*s) ++s;

    return s - p;
}

char* reverse(char* s)
{
    char c;
    int i = 0, j = strlen(s) - 1;

    while (j > i)
    {
        c = s[i];
        s[i++] = s[j];
        s[j--] = c;
    }

    return s;
}

char* itoa(int val, char* buf, int base)
{
    if (base < 2 && base != -10)
        return NULL;

    char* prefix = "";
    char digit;
    int i = 0;

    switch (base)
    {
        case   2:  prefix = "b0"; break;
        case   8:  prefix = "0";  break;
        case  16:  prefix = "x0"; break;
        case -10:  if (val < 0) prefix = "-"; break;
    }

    if (val > 0)
    {
        if (base == -10)
        {
            base = -base;
            if (val < 0)
                val = -val;
        }

        for (; val > 0; val /= base)
        {
            digit = val % base;
            buf[i++] = digit + (digit < 0xA ? 0x30 : 0x37);
        }
    }
    else
        buf[i++] = '0';

    strcpy(buf + i, prefix);
    reverse(buf);

    return buf;
}
