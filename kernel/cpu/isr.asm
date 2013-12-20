extern exceptionHandlers
extern irqHandlers

section .text
%macro exception 1
    global exception%1
    exception%1:
        %if (%1 != 8 && !(%1 >= 10 && %1 <= 14) && %1 != 17)
            push 0
        %endif
        push %1
        pusha
        mov ax, 0x10
        mov ds, ax
        mov es, ax

        mov ebx, esp
        mov esp, 0x7FFF0
        push ebx
        call [exceptionHandlers + (%1 * 4)]
        mov esp, ebx

        mov ax, 0x23
        mov ds, ax
        mov es, ax
        popa
        add esp, 8
        iret
%endmacro

%macro irq 1
    global irq%1
    irq%1:
        push 0
        push %1
        pusha
        mov ax, 0x10
        mov ds, ax
        mov es, ax

        mov ebx, esp
        mov esp, 0x7FFF0
        push ebx
        call [irqHandlers + (%1 * 4)]
        mov esp, ebx

        mov al, 0x20
        %if (%1 >= 8)
            out 0xA0, al
        %endif
        out 0x20, al

        mov ax, 0x23
        mov ds, ax
        mov es, ax
        popa
        add esp, 8
        iret
%endmacro

%assign i 0
%rep 32
    align 4
    exception i
    %assign i i+1
%endrep

%assign i 0
%rep 16
    align 4
    irq i
    %assign i i+1
%endrep
