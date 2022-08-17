#include "arch/x86_64/cpu_context.h"

	.global getContext
	.type getContext, @function
getContext:
	.cfi_startproc

	/* Callee saved: RBX, RBP, R12-R15  */
	movq %rbx, DW_X86_64_OFFSET_RBX(%rdi)
	movq %rbp, DW_X86_64_OFFSET_RBP(%rdi)
	movq %r12, DW_X86_64_OFFSET_R12(%rdi)
	movq %r13, DW_X86_64_OFFSET_R13(%rdi)
	movq %r14, DW_X86_64_OFFSET_R14(%rdi)
	movq %r15, DW_X86_64_OFFSET_R15(%rdi)

	/* x87 FPU env */
	fnstenv DW_X86_64_OFFSET_X87_ENV(%rdi)

	/* MXCSR reg */
	stmxcsr DW_X86_64_OFFSET_MXCSR(%rdi)

	/* Stack pointer */
	leaq 8(%rsp), %rax
	movq %rax, DW_X86_64_OFFSET_RSP(%rdi)

	/* Caller instruction pointer */
	movq 0(%rsp), %rax
	movq %rax, DW_X86_64_OFFSET_RIP(%rdi)

	xorq %rax, %rax
	retq
	.cfi_endproc
	.size getContext, . - getContext

	.global setContext
	.type setContext, @function
setContext:
	.cfi_startproc
	/* Restore stack pointer */
	movq DW_X86_64_OFFSET_RSP(%rdi), %rsp

	/* Push the return address on the stack */
	movq    DW_X86_64_OFFSET_RIP(%rdi),%rcx
	pushq   %rcx

	/* MXCST reg */
	ldmxcsr DW_X86_64_OFFSET_MXCSR(%rdi)

	/* x87 FPU env */
	fldenv DW_X86_64_OFFSET_X87_ENV(%rdi)

	/* Restore callee saved: RBX, RBP, R12-R15 */
	movq DW_X86_64_OFFSET_RBX(%rdi), %rbx
	movq DW_X86_64_OFFSET_RBP(%rdi), %rbp
	movq DW_X86_64_OFFSET_R12(%rdi), %r12
	movq DW_X86_64_OFFSET_R13(%rdi), %r13
	movq DW_X86_64_OFFSET_R14(%rdi), %r14
	movq DW_X86_64_OFFSET_R15(%rdi), %r15
	
	movq DW_X86_64_OFFSET_RAX(%rdi), %rax
	movq DW_X86_64_OFFSET_RDX(%rdi), %rdx

	retq
	.cfi_endproc
	.size setContext, . - setContext