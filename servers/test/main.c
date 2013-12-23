void main(void)
{
    asm volatile ("int $0x80");
    while (1);
}
