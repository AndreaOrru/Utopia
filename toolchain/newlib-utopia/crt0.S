.extern main
.extern exit

.global _start
_start:
    call main
    call exit
    .wait:
        hlt
        jmp .wait


.section .data
.global _TLSPtr
_TLSPtr:
    .long 0x600000
