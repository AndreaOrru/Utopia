#include <stdio.h>
#include <utopia.h>

int main(void)
{
    irq_subscribe(1);

    while(1)
    {
        irq_wait(1);

        printf("%d ", inb(0x60));
        fflush(stdout);
    }

    return 0;
}
