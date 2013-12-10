section .bss
gdtr:
    .limit:  resw 1
    .base:   resd 1

section .text
global gdt_flush
gdt_flush:
    mov eax, [esp + 4]
    mov [gdtr.base], eax
    mov ax, [esp + 8]
    mov [gdtr.limit], ax
    lgdt [gdtr]

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.reloadCS
    .reloadCS:
        ret
