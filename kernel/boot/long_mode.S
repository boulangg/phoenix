# Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
# The file is distributed under the MIT license
# The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE

.section .text
.code32

#define ASM_FILE
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

# Setting up the Paging

# # Init basic hierarchy
#_paging_init_hierarchy:
	movl $(kernel_pml4t-KERNEL_HIGH_VMA), %edi
	movl $(kernel_low_pdpt-KERNEL_HIGH_VMA), %eax
	orl $0x3, %eax
	movl %eax, (%edi)
	# Init High mem
	movl $(kernel_pdpt-KERNEL_HIGH_VMA), %eax
	add $(8*511), %edi
	orl $0x3, %eax
	movl %eax, (%edi)
	movl $(kernel_pdpt-KERNEL_HIGH_VMA), %edi
	movl $(kernel_pdt-KERNEL_HIGH_VMA), %eax
	orl $0x3, %eax
	add $(511*8), %edi
	movl %eax, (%edi)
	#
	movl $(kernel_low_pdpt-KERNEL_HIGH_VMA), %edi
	movl $(kernel_pdt-KERNEL_HIGH_VMA), %eax
	orl $0x3, %eax
	movl %eax, (%edi)
	movl $(kernel_pdt-KERNEL_HIGH_VMA), %edi
	movl $(kernel_pt-KERNEL_HIGH_VMA), %eax
	orl $0x3, %eax
	movl %eax, (%edi)

# # Init first 2 MB
#_paging_init_2MB:
	mov $(kernel_pt-KERNEL_HIGH_VMA), %edi
	mov $0x3, %ebx
	mov $512, %ecx
0:
	mov %ebx, (%edi)
	add $0x1000, %ebx
	add $8, %edi
	loop 0b

# # Enable PAE-paging
#_paging_init_PAE:
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
	mov $0x18, %eax
	mov %ax, %ds
	mov %ax, %es
	mov $0x00, %eax
	mov %ax, %fs
	mov %ax, %gs
	//sti
	movabsq $(boot), %rax
	callq *%rax

0:
	hlt
	jmp 0b

	cli
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
	jmp 0b

.section .data

.align 8
boot_gdtr:
    .word GDT_SIZE	# limit
    .quad (gdt-KERNEL_HIGH_VMA)		# base

noCPUIDMessage:
	.ascii "CPUID not supported by the processor\0"
noLongModeMessage:
	.ascii "64 bits mode not supported by the processor\0"
