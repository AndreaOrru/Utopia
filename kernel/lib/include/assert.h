#include "tty.h"  // ERROR.

#undef assert


#ifndef DEBUG
#define assert(e)

#else
#define assert(e)                                                        \
({                                                                       \
    if (!(e))                                                            \
        ERROR("%s:%u: failed assertion \"%s\"", __FILE__, __LINE__, #e); \
})

#endif
