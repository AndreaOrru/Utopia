#include <stdarg.h>
#include "string.h"
#include "term.h"

#define SIZE_X     80
#define SIZE_Y     25
#define DEFAULT_BG  0
#define DEFAULT_FG  7

static uint16_t* const VRAM = (uint16_t*)0xB8000;
static int cursor = 0;

static uint8_t bg = DEFAULT_BG;
static uint8_t fg = DEFAULT_FG;

static void update_cursor(void)
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

        for (int i = 0; i < cursor; i++)
            VRAM[i] = VRAM[i + SIZE_X];

        for (int i = 0; i < SIZE_X; i++)
            VRAM[i + cursor] = (bg << 12) | ' ';
    }

    VRAM[cursor++] = ((bg << 4 | (fg & 0x0F)) << 8) | c;
}

static void write(const char* s)
{
    for (int i = 0; s[i] != '\0'; i++)
        put(s[i]);
}

void clear_screen(void)
{
    for (int i = 0; i < SIZE_X * SIZE_Y; i++)
        VRAM[i] = (bg << 12) | ' ';

    cursor = 0;
    update_cursor();
}

void printf(const char* format, ...)
{
    static char buf[35];

    va_list args;
    va_start(args, format);

    uint8_t saved_bg = bg, saved_fg = fg;

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

            case '\e':
                switch (format[i+1])
                {
                    case 'B':
                        bg = format[i+2] == '1' ? format[i+3] + 0xA : format[i+3];
                        i += 3;
                        break;

                    case 'F':
                        fg = format[i+2] == '1' ? format[i+3] + 0xA : format[i+3];
                        i += 3;
                        break;

                    default:
                        bg = format[i+1] == '1' ? format[i+2] + 0xA : format[i+2];
                        fg = format[i+3] == '1' ? format[i+4] + 0xA : format[i+4];
                        i += 4;
                        break;
                }
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

    bg = saved_bg; fg = saved_fg;
    update_cursor();

    va_end(args);
}
