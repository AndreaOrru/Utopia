#pragma once

// Kernel space:
#define KERNEL_SPACE      0x00000000

#define INIT_STACK        0x00040000
#define INTERRUPT_STACK   0x00080000

#define FRAME_STACK       0x00200000
#define TMP_MAP           0x00600000

#define PCB_START         0x00700000
#define TCB_START         0x00900000

#define KERNEL_SPACE_END  0x20000000


// User space:
#define USER_SPACE        0x20000000

#define USER_STACKS       0x20000000
#define USER_TEXT         0x21000000

#define THREAD_EXIT       0xDEADC0DE

#define USER_SPACE_END    0xFFC00000


// Paging structs:
#define PAGING_STRUCTS    0xFFC00000
