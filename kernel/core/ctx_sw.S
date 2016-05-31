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
// registers that need to be saved upon a context switch:
// rax, rbx, rcx, rdx, rsp, rbp, rsi, rdi, r8-r15, st0-st7

// the xmm0-xxm15, mmx0-mmx7, x87 SW, x87 CW are saved by the
// floating point interrupt handler: since some user program don't use
// floating point register at all (or rarely), it is better to save
// them only when needed.
