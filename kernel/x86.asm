section .bss
gdtr:
    .limit:  resw 1
    .base:   resd 1

idtr:
    .limit:  resw 1
    .base:   resd 1

section .text
global gdt_load
gdt_load:
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

global tss_load
tss_load:
    mov ax, [esp + 4]
    ltr ax
    ret

global idt_load
idt_load:
    mov eax, [esp + 4]
    mov [idtr.base], eax
    mov ax, [esp + 8]
    mov [idtr.limit], ax

    lidt [idtr]
    ret

global enable_paging
enable_paging:
    mov eax, [esp + 4]
    mov cr3, eax

    mov eax, cr4
    or eax, 0b10010000
    mov cr4, eax

    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    ret
