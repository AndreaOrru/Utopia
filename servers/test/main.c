void main(void)
{
    int ret;
    asm volatile ("int $0x80" : "=a" (ret) : "0" (0), "c" ("%c %c %c %c"), "d" ('A'), "b" ('B'), "D" ('C'), "S" ('D'));

    while (1);
}
