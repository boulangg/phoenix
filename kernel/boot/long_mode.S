/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

.section .text
.code32

#define ASM_FILE
#include <include/constant.h>
#include "processor_struct.hpp"

.globl switch_to_long_mode
switch_to_long_mode:

# Detection of CPUID
#_detect_cpuid:
	pushfl
	pop %eax
	mov %eax, %ecx
	xor $(1 << 21), %eax		# Flip bit 21 of EFLAGS
	push %eax
	popfl
	pushfl
	pop %eax
	push %ecx
	popfl
	xor %eax, %ecx
	jz _no_cpuid

# Detection x86_64
#_detect_x86_64:
	mov $0x80000000, %eax
	cpuid
	cmp $0x80000001, %eax
	jb _no_long_mode

	mov $0x80000001, %eax
	cpuid
	test $(1 << 29), %edx
	jz _no_long_mode

#define PAGE_SIZE 0x1000
#define PAGE_MASK ~(PAGE_SIZE-1)

#Init linear mapping (64 Mo)
_init_virtual_mem:
	# init kernel_page_limit
	mov $(kernel_page_limit-KERNEL_HIGH_VMA), %eax
	mov $(_kernel_end-KERNEL_HIGH_VMA), %ecx
	mov %ecx, (%eax)

	# clear kernel_pml4t
	movl $(kernel_pml4t-KERNEL_HIGH_VMA), %edi
	mov $1024, %ecx
	mov $0, %ebx
0:
	mov %ebx, (%edi)
	add $4, %edi
	loop 0b

	#level 4
	movl $(kernel_pml4t-KERNEL_HIGH_VMA), %edi
	call _get_free_page
	movl %eax, %esi
	orl $0x3, %eax
	movl %eax, (%edi)
	addl $(8*256), %edi
	movl %eax, (%edi)

	#level 3
	movl %esi, %edi
	call _get_free_page
	movl %eax, %esi
	orl $0x3, %eax
	movl %eax, (%edi)

	#level 2
	movl %esi, %edi
	movl $32, %ecx	// counter (=64Mo)
	movl $0, %ebx	// start
0:
	push %ecx		// save current counter
	call _get_free_page
	movl %eax, %esi
	orl $0x3, %eax
	movl %eax, (%edi)	// save level 1 pointer into lvl 2 table
	push $(512)		// counter lvl 1
	push %ebx		// start_addr lvl 1
	push %esi		// page to fill in lvl 1
	#level 1
	call _init_page_linear_mapping_lvl1
	add $12, %esp
	pop %ecx		// restore %ecx

	add $0x8, %edi
	add $0x200000, %ebx
	loop 0b

	#init kernel mapping
_init_kernel_mem:
	movl $(kernel_pml4t-KERNEL_HIGH_VMA), %edi
	call _get_free_page
	movl %eax, %esi
	orl $0x3, %eax
	addl $(8*511), %edi
	movl %eax, (%edi)

	movl %esi, %edi
	movl $(kernel_pml4t-KERNEL_HIGH_VMA), %eax
	movl (%eax), %eax
	andl $(~0x3), %eax
	movl (%eax), %eax
	addl $(8*511), %edi
	movl %eax, (%edi)

	#init heap mapping
	# level 4
	movl $(kernel_pml4t-KERNEL_HIGH_VMA), %edi
	call _get_free_page
	movl %eax, %esi
	orl $0x3, %eax
	addl $(8*264), %edi
	movl %eax, (%edi)

	#level 3
	movl %esi, %edi
	call _get_free_page
	movl %eax, %esi
	orl $0x3, %eax
	movl %eax, (%edi)

	#level 2 (64 Ko)
	movl %esi, %edi
	call _get_free_page
	movl %eax, %esi
	orl $0x3, %eax
	movl %eax, (%edi)

	#level 1
	movl %esi, %edi
	movl $(16), %ecx
0:
	push %ecx
	call _get_free_page
	movl %eax, %esi
	orl $0x3, %eax
	movl %eax, (%edi)
	add $8, %edi
	pop %ecx
	loop 0b

	jmp _paging_init_PAE

# arg1 = page_addr, arg2 = start_addr (align on 4Ko), arg3 = nb_iteration, incr = 0x1000, flag = 0x3
_init_page_linear_mapping_lvl1:
	pushl %ebp
	movl %esp, %ebp
	push %ebx
	push %edi
	push %esi
	mov 8(%ebp), %edi
	mov 12(%ebp), %ebx
	andl $PAGE_MASK, %ebx
	orl $0x3, %ebx
	mov 16(%ebp), %ecx
0:
	mov %ebx, (%edi)
	add $PAGE_SIZE, %ebx
	add $8, %edi
	loop 0b
	pop %esi
	pop %edi
	pop %ebx
	pop %ebp
	ret

# return %eax = free page addr
_get_free_page:
	push %edi
	mov $(kernel_page_limit-KERNEL_HIGH_VMA), %edi
	mov (%edi), %eax
	mov $PAGE_SIZE, %ecx
	add (%edi), %ecx
	mov %ecx, (%edi)
	push %eax
	mov $1024, %ecx
	mov $0, %edx
0:
	mov %edx, (%eax)
	add $4, %eax
	loop 0b
	pop %eax
	pop %edi
	ret

# # Enable PAE-paging
_paging_init_PAE:
	mov %cr4, %eax
	or $(1 << 5), %eax
	mov %eax, %cr4

	movl $(kernel_pml4t-KERNEL_HIGH_VMA), %edi
    movl %edi, %cr3


# Switch to compatibility mode
# # Set LM-bit
#_compat_mode_LM:
	mov $0xC0000080, %ecx
	rdmsr
	or $(1 << 8), %eax
	wrmsr

# # Enable paging
#_compat_mode_paging:
	mov %cr0, %eax
	or $(1 << 31), %eax
	mov %eax, %cr0
	jmp _switch_to_64bit

# Switch to 64-bit mode
_switch_to_64bit:
# # Check EFER.LMA (bit 10) Long Mode Active
	movl $0xC0000080, %ecx
    rdmsr

	movl $(kernel_pml4t-KERNEL_HIGH_VMA), %edi
    movl %edi, %cr3

# # Initialize GDT
	movl $(gdt-KERNEL_HIGH_VMA), %eax
	# NULL descriptor
	movl $0x00000000, (%eax)
	addl $4, %eax
	movl $0x00000000, (%eax)
	addl $4, %eax
	# Empty Descriptor
	movl $0x00000000, (%eax)
	addl $4, %eax
	movl $0x00000000, (%eax)
	addl $4, %eax
	# KERNEL_CS Descriptor (64 bits mode)
	movl $0x00000000, (%eax)
	addl $4, %eax
	movl $0x00209800, (%eax)
	addl $4, %eax
	# KERNEL_DS Descriptor (64 bits mode)
	movl $0x00000000, (%eax)
	addl $4, %eax
	movl $0x00009200, (%eax)

# # Load GDT
	lgdt boot_gdtr
	jmpl $0x10, $_start64


_no_cpuid:
	mov $noCPUIDMessage, %eax
	call _print_asm
0:
	hlt
	jmp 0b

_no_long_mode:
	mov $noLongModeMessage, %eax
	call _print_asm
0:
	hlt
	jmp 0b

_print_asm:
	push %eax
	mov $0xB8000, %edi
	mov $0x0F, %ch		# Couleur Blanc sur Noir
0:
	mov (%eax), %cl
	cmp $0, %cl
	je 1f
	mov %cx, (%edi)
	add $1, %eax
	add $2, %edi
	jmp 0b
1:
	pop %eax
	ret


.code64
.globl _start64
_start64:
	cli
	#set kernel stack
	movq $(KERNEL_STACK_TOP), %rsp
	mov $0, %ebp

	#set rflags to 0
	mov $0, %rax
	push %rax
	popfq

	#set segments
	mov $0x18, %eax
	mov %ax, %ds
	mov %ax, %es
	mov $0x00, %eax
	mov %ax, %fs
	mov %ax, %gs
	movabsq $(boot), %rax
	callq *%rax

0:
	hlt
	jmp 0b

/*	cli
	mov $0x18, %eax
	mov %ax, %ds
	mov %ax, %es
	mov $0x00, %eax
	mov %ax, %fs
	mov %ax, %gs
	mov $0xB8000, %edi
	mov $0x1F201F201F201F20, %rax
	mov $500, %ecx
	rep stosq

0:
	hlt
	jmp 0b*/

.section .data

.align 8
boot_gdtr:
    .word GDT_SIZE	# limit
    .quad (gdt-KERNEL_HIGH_VMA)		# base

/*kernel_page_limit:
	.long (_kernel_end-KERNEL_HIGH_VMA)*/

noCPUIDMessage:
	.ascii "CPUID not supported by the processor\0"
noLongModeMessage:
	.ascii "64 bits mode not supported by the processor\0"
