#include <stdio.h>
#include <utopia.h>

int main(void)
{
    MBOX->tag.n  = 1;
    MBOX->reg[0] = 10;

    //send_receive(2, 2);
    printf("Ciao!");

    return 0;
}
