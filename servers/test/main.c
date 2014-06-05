#include <stdio.h>
#include <utopia.h>

void thread(void)
{
    printf(">>> ");
    fflush(stdout);

    return;
}

int main(void)
{
    thread_create(thread);

    while(1);

    return 0;
}
