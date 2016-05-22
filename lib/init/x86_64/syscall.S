/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

.text

#define SYSCALL(number, fn) \
	.global fn;				\
	fn:						\
		push %rbp;			\
		mov %rsp, %rbp;		\
		mov $number, %rax;	\
		syscall;			\
		pop %rbp;			\
		ret

SYSCALL(60, sys_exit)
