extern interruptHandlers
extern syscall_handler
extern state

section .text
%macro isr 1
    global isr%1
    isr%1:
        %if (%1 != 8 && !(%1 >= 10 && %1 <= 14) && %1 != 17)
            push 0
        %endif
        push %1
        pusha
        push ds
        push es

        mov ax, 0x10
        mov ds, ax
        mov es, ax

        mov [state], esp
        %if (%1 == 128)
            call syscall_handler
        %else
            call [interruptHandlers + (%1 * 4)]
        %endif
        mov esp, [state]

        %if (%1 >= 32 && %1 < 32 + 16)
            mov al, 0x20
            %if (%1 - 32 >= 8)
                out 0xA0, al
            %endif
            out 0x20, al
        %endif

        pop es
        pop ds
        popa
        add esp, 8
        iret
%endmacro

%assign i 0
%rep (32 + 16)
    align 4
    isr i
    %assign i i+1
%endrep

isr 128
