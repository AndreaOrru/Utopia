extern interruptHandlers
extern syscallHandlers
extern state

%define EXCEPTION(x)  (x <  32)
%define NO_ERROR(x)   (x != 8 && !(x >= 10 && x <= 14) && x != 17)
%define IRQ(x)        (x >= 32 && x < 32 + 16)
%define IRQ_SLAVE(x)  (x  - 32 >= 8)
%define SYSCALL(x)    (x == 128)

section .text
%macro isr 1
    global isr%1
    isr%1:
        %if NO_ERROR(%1)
            push 0
        %endif
        push %1
        pusha

        mov bp, 0x10
        mov ds, bp
        mov es, bp

        %if EXCEPTION(%1)
            mov [state], esp
        %endif
        %if SYSCALL(%1)
            push ebx
            push edx
            push ecx
            call [syscallHandlers + (eax * 4)]
            mov [esp + 40], eax
        %else
            call [interruptHandlers + (%1 * 4)]
        %endif
        mov esp, [state]

        %if IRQ(%1)
            mov al, 0x20
            %if IRQ_SLAVE(%1)
                out 0xA0, al
            %endif
            out 0x20, al
        %endif

        mov bp, 0x23
        mov ds, bp
        mov es, bp

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
