ENTRY(start)

SECTIONS
{
    . = 0x100000;

    .text BLOCK(4K):
    {
        *(.multiboot)
        *(.text)
    }

    .rodata BLOCK(4K):
    {
        *(.rodata)
    }

    .data BLOCK(4K):
    {
        *(.data)
    }

    .bss BLOCK(4K):
    {
        *(COMMON)
        *(.bss)
    }

    /DISCARD/ :
    {
        *(.comment)
        *(.eh_frame)
    }
}
