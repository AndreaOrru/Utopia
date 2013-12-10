MODALIGN  equ  (1 << 0)
MEMINFO   equ  (1 << 1)
FLAGS     equ  (MODALIGN | MEMINFO)
MAGIC     equ  0x1BADB002
CHECKSUM  equ -(MAGIC + FLAGS)

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .text
global _start
_start:
    mov esp, 0x7C00

    extern kmain
    call kmain

    cli
    hlt
