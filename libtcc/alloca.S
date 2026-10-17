.globl alloca
alloca:
    pop     %rdx
    mov     %rcx,%rax
    add     $15,%rax
    and     $-16,%rax
    jz      p3
p1:
    cmp     $4096,%rax
    jb      p2
    test    %rax,-4096(%rsp)
    sub     $4096,%rsp
    sub     $4096,%rax
    jmp p1
p2:
    sub     %rax,%rsp
    mov     %rsp,%rax
p3:
    push    %rdx
    ret
