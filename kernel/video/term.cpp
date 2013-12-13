#include <stdarg.h>
#include "string.h"
#include "x86.hpp"
#include "term.hpp"

namespace Term {

uint16_t* const VRAM = (uint16_t*)0xB8000;
const int SIZE_X = 80;
const int SIZE_Y = 25;
const uint8_t bg = 0;
const uint8_t fg = 7;

int cursor = 0;

inline void update_cursor()
{
    uint16_t h =  (cursor & 0xFF00)       | 0x0E;
    uint16_t l = ((cursor & 0x00FF) << 8) | 0x0F;

    outw(0x3D4, h);
    outw(0x3D4, l);
}

inline void put(char c)
{
    if (cursor == SIZE_X * SIZE_Y)
    {
        cursor -= SIZE_X;

        memmove(&VRAM[0], &VRAM[SIZE_X], cursor);

        for (int i = 0; i < SIZE_X; i++)
            VRAM[i + cursor] = ' ';
    }

    VRAM[cursor++] = (bg << 12) | (fg << 8) | c;
}

inline void write(const char* s)
{
    for (int i = 0; s[i] != '\0'; i++)
        put(s[i]);
}

void clear()
{
    for (int i = 0; i < SIZE_X * SIZE_Y; i++)
        VRAM[i] = ' ';

    cursor = 0;
    update_cursor();
}

void printf(const char* format, ...)
{
    static char buf[35];

    if (!format)
        return;

    va_list args;
    va_start(args, format);

    for (int i = 0; format[i] != '\0'; i++)
        switch (format[i])
        {
            case '\n':
                do
                    put(' ');
                while (cursor % SIZE_X);
                break;

            case '\t':
                do
                    put(' ');
                while (cursor % 4);
                break;

            case '%':
                switch (format[++i])
                {
                    case 'x':  write(itoa(va_arg(args, uint32_t), buf, 16));  break;
                    case 'u':  write(itoa(va_arg(args, uint32_t), buf, 10));  break;
                    case 'd':
                    case 'i':  write(itoa(va_arg(args, uint32_t), buf, -10)); break;
                    case 'o':  write(itoa(va_arg(args, uint32_t), buf, 8));   break;
                    case 'b':  write(itoa(va_arg(args, uint32_t), buf, 2));   break;
                    case 'c':  put  (va_arg(args, uint32_t));                 break;
                    case 's':  write(va_arg(args, char*));                    break;
                    case '%':  put  ('%');                                    break;
                }
                break;

            default:
                put(format[i]);
                break;
        }

    update_cursor();

    va_end(args);
}

}
