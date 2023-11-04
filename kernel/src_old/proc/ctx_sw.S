    .text
    .globl ctx_sw
ctx_sw:
	pushfq
	pop %rcx
    mov %rdi, %rax
    mov %rbx, (%rax)
    mov %rsp, 8(%rax)
    mov %rbp, 16(%rax)
    mov %r12, 24(%rax)
    mov %r13, 32(%rax)
    mov %r14, 40(%rax)
    mov %r15, 48(%rax)
    mov %rcx, 56(%rax)

    mov %rsi, %rax
    mov (%rax), %rbx
    mov 8(%rax), %rsp
    mov 16(%rax), %rbp
    mov 24(%rax), %r12
    mov 32(%rax), %r13
    mov 40(%rax), %r14
    mov 48(%rax), %r15
    mov 56(%rax), %rcx
	mov 64(%rax), %rdx

	mov %rdx, %cr3
	push %rcx
	popfq
    ret

    .globl start_idle
start_idle:

    mov %rdi, %rax
    mov (%rax), %rbx
    mov 8(%rax), %rsp
    mov 16(%rax), %rbp
    mov 24(%rax), %r12
    mov 32(%rax), %r13
    mov 40(%rax), %r14
    mov 48(%rax), %r15
	mov 64(%rax), %rdx

	mov %rdx, %cr3
    ret
// registers that need to be saved upon a context switch:
// rbx, rsp, rbp, r12, r13, r14, r15, rflags (on the stack)

// the xmm0-xxm15, mmx0-mmx7, x87 SW, x87 CW are saved by the
// floating point interrupt handler: since some user program don't use
// floating point register at all (or rarely), it is better to save
// them only when needed.

	.globl save_regs
save_regs:
    mov %rdi, %rax
    mov %rbx, (%rax)
    mov %rsp, 8(%rax)
    mov %rbp, 16(%rax)
    mov %r12, 24(%rax)
    mov %r13, 32(%rax)
    mov %r14, 40(%rax)
    mov %r15, 48(%rax)
    ret

	.globl switch_to_user_mode
switch_to_user_mode:
	pop %rcx	// entry point
	//mov (%rcx), %rax
	pushfq
	pop %r11	// rflags
	sysretq

	.globl load_new_task
load_new_task:
	mov %rdi, %rax
    mov (%rax), %rbx
    mov 8(%rax), %rsp
    mov 16(%rax), %rbp
    mov 24(%rax), %r12
    mov 32(%rax), %r13
    mov 40(%rax), %r14
    mov 48(%rax), %r15
    mov 56(%rax), %rcx
    mov 64(%rax), %rdx

	mov %rdx, %cr3
	push %rcx
	popfq
    ret
