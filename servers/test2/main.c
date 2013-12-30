int main(void)
{
    int* utcb = *(int**)0x600000;

    int ret;
    asm volatile ("int $0x80" : "=a" (ret) : "0" (1), "c" (0), "d" (1));

    return 0;
}
