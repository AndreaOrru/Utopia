void main(void)
{
    int ret;
    asm volatile ("int $0x80" : "=a" (ret) : "0" (0), "b" ('A'));
    asm volatile ("int $0x80" : "=a" (ret) : "0" (0), "b" ('B'));

    while (1);
}
