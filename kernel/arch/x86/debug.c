#include "arch/x86/asm.h"
#include "debug.h"

inline void hang(void)
{
    cli();
    hlt();
}
