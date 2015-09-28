#include <stdlib.h>
#include <string.h>

char* itoa(unsigned val, char* buf, int base)
{
    if ((base < 2 || base > 36) && base != -10)
    {
        *buf = '\0';
        return buf;
    }

    char* ptr = buf;
    switch (base)
    {
        case   2:  strcpy(ptr, "0b"); ptr += 2; break;
        case   8:  strcpy(ptr, "0");  ptr += 1; break;
        case  16:  strcpy(ptr, "0x"); ptr += 2; break;
        case -10:  base = -base; if ((int) val < 0) { val = -val; *ptr++ = '-'; } break;
    }
    char* low = ptr;

    if (val != 0)
        do
        {
            *ptr++ = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[val % base];
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
