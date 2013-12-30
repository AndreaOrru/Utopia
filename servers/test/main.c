int main(void)
{
    unsigned* utcb = *(unsigned**)0x600000;

    int ret;
    utcb[0] = 1;
    utcb[1] = 10;

    asm volatile ("int $0x80" : "=a" (ret) : "0" (1), "c" (2), "d" (2));

    return 0;
}
