    .text
    .globl ctx_sw
ctx_sw:

    mov %rdi, %rax
    mov %rbx, (%rax)
    mov %rsp, 8(%rax)
    mov %rbp, 16(%rax)
    mov %r12, 24(%rax)
    mov %r13, 32(%rax)
    mov %r14, 40(%rax)
    mov %r15, 48(%rax)

    mov %rsi, %rax
    mov (%rax), %rbx
    mov 8(%rax), %rsp
    mov 16(%rax), %rbp
    mov 24(%rax), %r12
    mov 32(%rax), %r13
    mov 40(%rax), %r14
    mov 48(%rax), %r15

    ret
