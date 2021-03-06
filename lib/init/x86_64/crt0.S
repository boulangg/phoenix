/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

.section .text

.global _start
_start:
	# Set up end of the stack frame linked list.
	movq $0, %rbp
	pushq %rbp # rip=0
	pushq %rbp # rbp=0
	movq %rsp, %rbp

	# We need those in a moment when we call main.
	# pushq %rsi
	# pushq %rdi

	# Run the global constructors.
	call _init

	# Prepare signals, memory allocation, stdio and such.
	# call initialize_standard_library
	call init_default_std

	# get argc and argv
	mov 16(%rbp), %rdi
	mov %rbp, %rsi
	addq $24, %rsi

	# Restore argc and argv.
	#popq %rdi
	#popq %rsi

	# Run main
	call main

	# Terminate the process with the exit code.
	movl %eax, %edi
	call _fini
	call sys_exit

0:
	loop 0b
