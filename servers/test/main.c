void main(void)
{
    int ret;
    asm volatile ("int $0x80" : "=a" (ret) : "0" (7));

    while (1);
}
