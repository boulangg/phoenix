.section .text
.code32

#define ASM_FILE
#include "processor_struct.h"
#define GDT_SIZE (GDT_ENTRIES*8-1)
#define IDT_SIZE (IDT_ENTRIES*16-1)

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
# # Clear the tables
#_paging_clear_tables:
	mov $pml4t, %edi
	mov %edi, %cr3
	xor %eax, %eax
	mov $1024, %ecx
	rep stosl
	mov $pdpt, %edi
	xor %eax, %eax
	mov $1024, %ecx
	rep stosl
	mov $pdt, %edi
	xor %eax, %eax
	mov $1024, %ecx
	rep stosl
	mov $pt, %edi
	xor %eax, %eax
	mov $1024, %ecx
	rep stosl

# # Init basic hierarchy
#_paging_init_hierarchy:
	movl $pml4t, %edi
	movl $pdpt, %eax
	orl $0x3, %eax
	movl %eax, (%edi)
	movl $pdpt, %edi
	movl $pdt, %eax
	orl $0x3, %eax
	movl %eax, (%edi)
	movl $pdt, %edi
	movl $pt, %eax
	orl $0x3, %eax
	movl %eax, (%edi)

# # Init first 2 MB
#_paging_init_2MB:
	mov $pt, %edi
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

	movl $pml4t, %edi
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

# # Initialize GDT
	movl $gdt, %eax
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
	movl $0x00009000, (%eax)

# # Load GDT
	lgdt gdtr
	ljmp $0x10, $_start64


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
	call boot

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

.align 8
gdtr:
    .word GDT_SIZE	# limit
    .quad gdt		# base

noCPUIDMessage:
	.ascii "CPUID not supported by the processor\0"
noLongModeMessage:
	.ascii "64 bits mode not supported by the processor\0"
