void __faststorefence(void)
{
    __asm__("lock; orl $0,(%%rsp)" ::: "memory");
}
