.section .text
.code64

.global get_rdtsc
get_rdtsc:
    push %rbp
    mov %rsp, %rbp
    rdtsc
    shl $0x20, %rdx
    or %rdx, %rax
    leave
