#include <utopia.h>

int main(void)
{
    MBOX->tag.n  = 0;
    MBOX->reg[0] = 10;

    send_receive(2, 2);

    return 0;
}
