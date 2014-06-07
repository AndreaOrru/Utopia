#include <malloc.h>
#include <utopia.h>
#include "video.h"

int main(void)
{
    int i = 0;
    uint16_t* video = memory_map((void*)0xB8000, 0x8000, true);

    while(1)
    {
        send_receive(0, -1);
        video[i++] = ((char) MBOX->reg[0]) | 0xF000;
    }

    return 0;
}
