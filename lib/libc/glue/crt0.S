.section .text

.global _start
.global main
_start:
	# Set up end of the stack frame linked list.
	movq $0, %rbp
	pushq %rbp # rip=0
	pushq %rbp # rbp=0
	movq %rsp, %rbp

	# Run __libc_start_main
	mov $(main), %rdi		# main
	mov 0x10(%rbp), %rsi	# argc
	mov %rbp, %rdx			# argv
	add $0x18, %rdx
	mov 0x0(%rsp), %rcx		# init
	mov 0x0(%rsp), %r8		# fini
	mov 0x0(%rsp), %r9		# rtld_fini

	call __libc_start_main

0:
	loop 0b

.section .note.GNU-stack,"",@progbits
