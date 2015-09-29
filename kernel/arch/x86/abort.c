#include <asm.h>     // cli, hlt.
#include <stdlib.h>

inline void abort(void)
{
    cli();
    hlt();
    __builtin_unreachable();
}
