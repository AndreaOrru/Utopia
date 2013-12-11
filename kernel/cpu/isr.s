section .text
exception_common:
    jmp $

%macro exception 1
    global exception%1
    exception%1:
        cli
        %if (%1 != 8 && !(%1 >= 10 && %1 <= 14) && %1 != 17)
            push 0
        %endif
        push %1
        jmp exception_common
%endmacro

%assign i 0
%rep 32
    exception i
    %assign i i+1
%endrep
