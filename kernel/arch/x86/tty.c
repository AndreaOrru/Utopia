#include <stddef.h>
#include "arch/x86/asm.h"
#include "vga.h"
#include "tty.h"

static VGAEntry* const vram = (VGAEntry*) 0xB8000;
static size_t cursor;
VGAColor tty_fg, tty_bg;

void tty_init(void)
{
    cursor = 0;
    outb(0x3D4, 0xA);
    outb(0x3D5, 1 << 5);

    tty_fg = COLOR_LIGHTGREY;
    tty_bg = COLOR_BLACK;

    tty_clear();
}

void tty_clear(void)
{
    for (size_t i = 0; i < VGA_HEIGHT * VGA_WIDTH; i++)
        vram[cursor++] = (VGAEntry) { ' ', tty_fg, tty_bg };

    cursor = 0;
}

void tty_putchar(char c)
{
    if (cursor == VGA_HEIGHT * VGA_WIDTH)
    {
        for (size_t i = 0; i < cursor; i++)
            vram[i] = vram[i + VGA_WIDTH];

        for (size_t i = 0; i < VGA_WIDTH; i++)
            vram[i + cursor] = (VGAEntry) { ' ', tty_fg, tty_bg };
    }

    switch (c)
    {
        case '\n':
            do
                tty_putchar(' ');
            while (cursor % VGA_WIDTH);
            break;

        case '\t':
            do
                tty_putchar(' ');
            while (cursor % 4);
            break;

        default:
            vram[cursor++] = (VGAEntry) { c, tty_fg, tty_bg };
    }
}
