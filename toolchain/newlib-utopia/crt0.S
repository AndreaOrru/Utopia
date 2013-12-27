.global _start
.extern main
.extern exit

_start:
    call main
    call exit
    .wait:
        hlt
        jmp .wait
