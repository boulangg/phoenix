# Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
# The file is distributed under the MIT license
# The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE

#define ASM_FILE
#include "multiboot2.h"

#define BOOT_MAGIC	MULTIBOOT2_HEADER_MAGIC
#define BOOT_ARCHI	MULTIBOOT_ARCHITECTURE_I386
#define BOOT_HDLEN  multiboot_header_end - multiboot_header_start
#define BOOT_CHECK 	-(BOOT_MAGIC + BOOT_ARCHI + BOOT_HDLEN)

# Multiboot header
.section .multiboot
.align 8
multiboot_header_start:
multiboot_magic_start:
	.long BOOT_MAGIC
	.long BOOT_ARCHI
	.long BOOT_HDLEN
	.long BOOT_CHECK
multiboot_magic_end:
	.short MULTIBOOT_HEADER_TAG_END
	.short 0
	.long 8
multiboot_header_end:

# Default stack
.section .bootstrap_stack, "aw", @nobits
.globl stack_top
stack_bottom:
	.skip 16384 # 16 KiB
stack_top:

# Kernel entry point _start
.section .text
.global _start
_start:
	# Set default stack
	movl $stack_top, %esp

	# Save multiboot_info
	movl %eax, multiboot_magic
	movl %ebx, multiboot_info

	# Call main function in C
	call switch_to_long_mode

	# Infinite loop
0:
	hlt
	jmp 0b

# Set the size of the _start symbol to the current location '.' minus its start.
# This is useful when debugging or when you implement call tracing.
#.size _start, . - _start
.global multiboot_info
.global multiboot_magic
	.comm multiboot_magic, 4
	.comm multiboot_info, 4
