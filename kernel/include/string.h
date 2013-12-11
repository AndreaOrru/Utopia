#pragma once
#include <stddef.h>

#if defined(__cplusplus)
extern "C"
{
#endif

int    memcmp (const void* s1, const void* s2, size_t n);
void*  memcpy (void* dest, const void* src, size_t n);
void*  memmove(void* dest, const void* src, size_t n);
void*  memset (void* s, int c, size_t n);
char*  strcat (char* dest, const char* src);
int    strcmp (const char* s1, const char* s2);
char*  strcpy (char* dest, const char* src);
size_t strlen (const char* s);
char*  reverse(char* s);
char*  itoa   (int val, char* buf, int base);

#if defined(__cplusplus)
}
#endif
