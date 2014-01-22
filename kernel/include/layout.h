#pragma once

#define KERNEL_SPACE      0x00000000
#define FRAME_STACK       0x00200000
#define FRAME_STACK_END   0x00400000
#define TMP_MAP           0x00500000
#define TLS_PTR           0x00600000
#define PCB_START         0x00700000
#define TCB_START         0x10000000
#define KERNEL_TLS        0x20000000
#define KERNEL_SPACE_END  0x30000000

#define USER_SPACE        0x30000000
#define USER_STACKS       0x30000000
#define USER_TLS          0x34000000
#define USER_HEAP         0x38000000
#define USER_TEXT         0x40000000
#define USER_SPACE_END    0xFFC00000

#define PAGING_STRUCTS    0xFFC00000
