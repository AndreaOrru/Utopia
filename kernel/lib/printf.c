#include <assert.h>  // assert.
#include <stdarg.h>  // va_*.
#include <stdlib.h>  // NULL, size_t, itoa.
#include "tty.h"     // tty_put*.
#include "vga.h"     // VGAColor.
#include <stdio.h>

extern VGAColor tty_fg, tty_bg;

void printf(const char* format, ...)
{
    assert(format != NULL);
    static char buf[35];

    va_list args;
    va_start(args, format);

    VGAColor saved_fg = tty_fg;
    VGAColor saved_bg = tty_bg;

    for (size_t i = 0; format[i] != '\0'; i++)
        switch (format[i])
        {
            case '\e':
                switch (format[i+1])
                {
                    case 'B':
                        tty_bg = format[i+2] == '1' ? format[i+3] + 0xA : format[i+3];
                        i += 3;
                        break;

                    case 'F':
                        tty_fg = format[i+2] == '1' ? format[i+3] + 0xA : format[i+3];
                        i += 3;
                        break;

                    default:
                        tty_bg = format[i+1] == '1' ? format[i+2] + 0xA : format[i+2];
                        tty_fg = format[i+3] == '1' ? format[i+4] + 0xA : format[i+4];
                        i += 4;
                }
                break;

            case '%':
                switch (format[++i])
                {
                    case 'x':  tty_puts(itoa(va_arg(args, unsigned), buf, 16));  break;
                    case 'u':  tty_puts(itoa(va_arg(args, unsigned), buf, 10));  break;
                    case 'd':
                    case 'i':  tty_puts(itoa(va_arg(args, int     ), buf, -10)); break;
                    case 'o':  tty_puts(itoa(va_arg(args, unsigned), buf, 8));   break;
                    case 'b':  tty_puts(itoa(va_arg(args, unsigned), buf, 2));   break;
                    case 'c':  tty_putchar  (va_arg(args, unsigned));            break;
                    case 's':  tty_puts     (va_arg(args, const char*));         break;
                    case '%':  tty_putchar  ('%');                               break;
                }
                break;

            default:
                tty_putchar(format[i]);
        }

    tty_bg = saved_bg;
    tty_fg = saved_fg;

    va_end(args);
}
