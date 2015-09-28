#pragma once

#define VGA_WIDTH   80
#define VGA_HEIGHT  25

typedef enum
{
    COLOR_BLACK        = 0,
    COLOR_BLUE         = 1,
    COLOR_GREEN        = 2,
    COLOR_CYAN         = 3,
    COLOR_RED          = 4,
    COLOR_MAGENTA      = 5,
    COLOR_BROWN        = 6,
    COLOR_LIGHTGREY    = 7,
    COLOR_DARKGREY     = 8,
    COLOR_LIGHTBLUE    = 9,
    COLOR_LIGHTGREEN   = 10,
    COLOR_LIGHTCYAN    = 11,
    COLOR_LIGHTRED     = 12,
    COLOR_LIGHTMAGENTA = 13,
    COLOR_LIGHTBROWN   = 14,
    COLOR_WHITE        = 15,
} VGAColor;

typedef struct
{
    char c;
    VGAColor fg : 4;
    VGAColor bg : 4;
} __attribute__((packed)) VGAEntry;
