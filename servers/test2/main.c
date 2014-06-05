#include <utopia.h>

void thread(void)
{
    put('A');

    return;
}

int main(void)
{
    thread_create(thread);

    return 0;
}
