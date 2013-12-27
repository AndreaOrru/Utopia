#include <string.h>

int main(void)
{
    int a[10];
    int b[10];

    memcpy(b, a, 10 * sizeof(char));

    return 0;
}
