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

.global default_handler
default_handler:
	iretq

.global reserved_handler
reserved_handler:
	iretq

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
