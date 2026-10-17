.globl __chkstk
__chkstk:
    xchg    (%rsp),%rbp
    push    %rbp
    lea     8(%rsp),%rbp
    push    %rcx
    mov     %rbp,%rcx
    movslq  %eax,%rax
P0:
    sub     $4096,%rcx
    test    %rax,(%rcx)
    sub     $4096,%rax
    cmp     $4096,%rax
    jge     P0
    sub     %rax,%rcx
    test    %rax,(%rcx)
    mov     %rsp,%rax
    mov     %rcx,%rsp
    mov     (%rax),%rcx
    jmp     *8(%rax)
