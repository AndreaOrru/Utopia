#include <stdint.h>
#include "string.h"

int memcmp(const void* s1, const void* s2, size_t n)
{
    const uint8_t* p1 = (const uint8_t*)s1;
    const uint8_t* p2 = (const uint8_t*)s2;

    for (; n--; p1++, p2++)
    {
	    uint8_t u1 = *p1, u2 = *p2;

	    if (u1 != u2)
	        return (u1 - u2);
    }

    return 0;
}

void* memcpy(void* dest, const void* src, size_t n)
{
    const char* s = src;
          char* d = dest;

    while (n--)
        *d++ = *s++;

    return dest;
}

void* memmove(void* dest, const void* src, size_t n)
{
    const uint8_t* s = src;
          uint8_t* d = dest;

    if (s < d)
        for (d += n, s += n; n--;)
            *--d = *--s;
    else
        while (n--)
            *d++ = *s++;

    return dest;
}

void* memset(void* s, int c, size_t n)
{
    uint8_t* p = s;

    while (n--)
        *p++ = (uint8_t)c;

    return s;
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
    const char* p = s;

    while (*s) ++s;

    return s - p;
}

char* itoa(int val, char* buf, int base)
{
    if ((base < 2 || base > 36) && base != -10)
    {
        *buf = '\0';
        return buf;
    }

    char *ptr = buf;
    switch (base)
    {
        case   2:  strcpy(ptr, "0b"); ptr += 2; break;
        case   8:  strcpy(ptr, "0");  ptr += 1; break;
        case  16:  strcpy(ptr, "0x"); ptr += 2; break;
        case -10:  base = -base; if (val < 0) *ptr++ = '-'; break;
    }
    char* low = ptr;

    if (val != 0)
        do
        {
            *ptr++ = "ZYXWVUTSRQPONMLKJIHGFEDCBA9876543210123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[35 + val % base];
            val /= base;
        }
        while (val);
    else
        *ptr++ = '0';

    *ptr-- = '\0';
    while (low < ptr)
    {
        char c = *low;
        *low++ = *ptr;
        *ptr-- = c;
    }

    return buf;
}
