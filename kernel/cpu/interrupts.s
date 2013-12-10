section .text
isr_common:
    jmp $

%macro isr 1
    global isr%1
    isr%1:
        cli
        %if (%1 != 8 && !(%1 >= 10 && %1 <= 14) && %1 != 17)
            push 0
        %endif
        push %1
        jmp isr_common
%endmacro

%assign i 0
%rep 32
    isr i
    %assign i i+1
%endrep
