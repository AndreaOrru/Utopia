#include <utopia.h>

void thread(void)
{
    put('A');

    return;
}

int main(void)
{
    thread_create(thread);

    while(1);

    return 0;
}
