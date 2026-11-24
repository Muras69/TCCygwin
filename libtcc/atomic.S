        .text
        .global __atomic_load_1
        .type   __atomic_load_1, %function
__atomic_load_1:
        movzbl (%rcx),%eax
        ret
	.size   __atomic_load_1, .-__atomic_load_1
        .global __atomic_load_2
        .type   __atomic_load_2, %function
__atomic_load_2:
        movzwl (%rcx),%eax
        ret
	.size   __atomic_load_2, .-__atomic_load_2
        .global __atomic_load_4
        .type   __atomic_load_4, %function
__atomic_load_4:
        mov    (%rcx),%eax
        ret
	.size   __atomic_load_4, .-__atomic_load_4
        .global __atomic_load_8
        .type   __atomic_load_8, %function
__atomic_load_8:
        mov    (%rcx),%rax
        ret
	.size   __atomic_load_8, .-__atomic_load_8
        .global __atomic_store_1
        .type   __atomic_store_1, %function
__atomic_store_1:
        xchg   %dl,(%rcx)
        ret
	.size   __atomic_store_1, .-__atomic_store_1
        .global __atomic_store_2
        .type   __atomic_store_2, %function
__atomic_store_2:
        xchg   %dx,(%rcx)
        ret
	.size   __atomic_store_2, .-__atomic_store_2
        .global __atomic_store_4
        .type   __atomic_store_4, %function
__atomic_store_4:
        xchg   %edx,(%rcx)
        ret
	.size   __atomic_store_4, .-__atomic_store_4
        .global __atomic_store_8
        .type   __atomic_store_8, %function
__atomic_store_8:
        xchg   %rdx,(%rcx)
        ret
	.size   __atomic_store_8, .-__atomic_store_8
        .global __atomic_compare_exchange_1
        .type   __atomic_compare_exchange_1, %function
__atomic_compare_exchange_1:
        movzbl (%rdx),%eax
        lock cmpxchg %r8b,(%rcx)
        sete   %cl
        je      .L___atomic_compare_exchange_1_00f
        mov    %al,(%rdx)
.L___atomic_compare_exchange_1_00f:
        mov    %ecx,%eax
        ret
	.size   __atomic_compare_exchange_1, .-__atomic_compare_exchange_1
        .global __atomic_compare_exchange_2
        .type   __atomic_compare_exchange_2, %function
__atomic_compare_exchange_2:
        movzwl (%rdx),%eax
        lock cmpxchg %r8w,(%rcx)
        sete   %cl
        je      .L___atomic_compare_exchange_2_011
        mov    %ax,(%rdx)
.L___atomic_compare_exchange_2_011:
        mov    %ecx,%eax
        ret
	.size   __atomic_compare_exchange_2, .-__atomic_compare_exchange_2
        .global __atomic_compare_exchange_4
        .type   __atomic_compare_exchange_4, %function
__atomic_compare_exchange_4:
        mov    (%rdx),%eax
        lock cmpxchg %r8d,(%rcx)
        sete   %cl
        je      .L___atomic_compare_exchange_4_00e
        mov    %eax,(%rdx)
.L___atomic_compare_exchange_4_00e:
        mov    %ecx,%eax
        ret
	.size   __atomic_compare_exchange_4, .-__atomic_compare_exchange_4
        .global __atomic_compare_exchange_8
        .type   __atomic_compare_exchange_8, %function
__atomic_compare_exchange_8:
        mov    (%rdx),%rax
        lock cmpxchg %r8,(%rcx)
        sete   %cl
        je      .L___atomic_compare_exchange_8_010
        mov    %rax,(%rdx)
.L___atomic_compare_exchange_8_010:
        mov    %ecx,%eax
        ret
	.size   __atomic_compare_exchange_8, .-__atomic_compare_exchange_8
        .global __atomic_test_and_set_1
        .type   __atomic_test_and_set_1, %function
__atomic_test_and_set_1:
        mov    $0x1,%eax
        xchg   %al,(%rcx)
        ret
	.size   __atomic_test_and_set_1, .-__atomic_test_and_set_1
        .global __atomic_test_and_set_2
        .type   __atomic_test_and_set_2, %function
__atomic_test_and_set_2:
        mov    $0x1,%eax
        xchg   %al,(%rcx)
        ret
	.size   __atomic_test_and_set_2, .-__atomic_test_and_set_2
        .global __atomic_test_and_set_4
        .type   __atomic_test_and_set_4, %function
__atomic_test_and_set_4:
        mov    $0x1,%eax
        xchg   %al,(%rcx)
        ret
	.size   __atomic_test_and_set_4, .-__atomic_test_and_set_4
        .global __atomic_test_and_set_8
        .type   __atomic_test_and_set_8, %function
__atomic_test_and_set_8:
        mov    $0x1,%eax
        xchg   %al,(%rcx)
        ret
	.size   __atomic_test_and_set_8, .-__atomic_test_and_set_8
        .global atomic_thread_fence
        .type   atomic_thread_fence, %function
atomic_thread_fence:
        lock orq $0x0,(%rsp)
        ret
	.size   atomic_thread_fence, .-atomic_thread_fence
        .global atomic_signal_fence
        .type   atomic_signal_fence, %function
atomic_signal_fence:
        ret
	.size   atomic_signal_fence, .-atomic_signal_fence
        .global atomic_flag_test_and_set
        .type   atomic_flag_test_and_set, %function
atomic_flag_test_and_set:
        mov    $0x1,%eax
        xchg   %al,(%rcx)
        ret
	.size   atomic_flag_test_and_set, .-atomic_flag_test_and_set
        .global atomic_flag_test_and_set_explicit
        .type   atomic_flag_test_and_set_explicit, %function
atomic_flag_test_and_set_explicit:
        mov    $0x1,%eax
        xchg   %al,(%rcx)
        ret
	.size   atomic_flag_test_and_set_explicit, .-atomic_flag_test_and_set_explicit
        .global atomic_flag_clear
        .type   atomic_flag_clear, %function
atomic_flag_clear:
        xor    %eax,%eax
        xchg   %al,(%rcx)
        ret
	.size   atomic_flag_clear, .-atomic_flag_clear
        .global atomic_flag_clear_explicit
        .type   atomic_flag_clear_explicit, %function
atomic_flag_clear_explicit:
        xor    %eax,%eax
        xchg   %al,(%rcx)
        ret
	.size   atomic_flag_clear_explicit, .-atomic_flag_clear_explicit
