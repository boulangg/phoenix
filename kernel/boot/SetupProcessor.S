/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#define ASM_FILE
#include <include/constant.h>
#include "SetupProcessor.hpp"

.section .text
.code64

.global set_TSS
set_TSS:
	mov %edi, tr
	ltr tr
	ret

.global set_IDT
set_IDT:
	mov %edi, idtr
	mov %esi, idtr+2
	lidt idtr
	ret

.global set_GDT
set_GDT:
	mov %edi, gdtr
	mov %rsi, gdtr+2
	lgdt gdtr
	ret

.global set_CR3
set_CR3:
	mov %rdi, %cr3
	ret

.global load_syscall
load_syscall:
	push %rcx
	push %rdx
	mov %edi, %eax
	shr $32, %rdi
	mov %edi, %edx
	mov $0xC0000081, %ecx	// START
	wrmsr
	mov %esi, %eax
	shr $32, %rsi
	mov %esi, %edx
	mov $0xC0000082, %ecx	// LSTAR
	wrmsr
	pop %rdx
	mov %edx, %eax
	shr $32, %rdx
	mov $0xC0000083, %ecx	// CSTAR
	wrmsr
	pop %rax
	mov $0, %edx
	mov $0xC0000084, %ecx	// SFMASK
	wrmsr
	ret

.global enable_syscall
enable_syscall:
	push %rcx
	push %rdx
	push %rax
# set bit 0 (SCE) of EFER register for syscall extension
	mov $0xC0000080, %ecx
    rdmsr
    or $0x1, %eax
    wrmsr
# set bit 16 (FSGSBASE) of cr4 register for [rd|wr][fs|gs] instructions
	mov %cr4, %rax
	or $0x10, %rax
	mov %rax, %cr4
    pop %rax
    pop %rdx
    pop %rcx
    ret

.global default_handler
default_handler:
	iretq

.global reserved_handler
reserved_handler:
	iretq

.global syscall64_handler
syscall64_handler:
	wrgsbase %rsp					// Save user %rsp in gsbase
	mov $KERNEL_SYSCALL_TOP, %rsp
	push %r11
	rdgsbase %r11
	push %r11
	push %rcx						// rcx contain user rip
	mov %r10, %rcx
	push %rax						// rax contains the number of the function (7th argument on the stack according to the System V ABI)
	call syscall64
	pop %rcx
	pop %rcx
	pop %r11
	wrgsbase %r11
	pop %r11
	rdgsbase %rsp					// Restore user %rsp
	sysret


.section .data

.align 8
gdtr:
    .word GDT_SIZE	# limit
    .quad (gdt-KERNEL_HIGH_VMA)		# base

.align 8
idtr:
    .word IDT_SIZE	# limit
    .quad idt		# base

.align 8
tr:
	.word 0x00		# selector
