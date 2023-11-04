.section .text

.global set_GDT
set_GDT:
	mov %edi, gdtr
	mov %rsi, gdtr+2
	lgdt gdtr
	ret

.global set_IDT
set_IDT:
	mov %edi, idtr
	mov %esi, idtr+2
	lidt idtr
	ret

.section .data

.align 8
gdtr:
    .word 0	# limit
    .quad 0	# base

.align 8
idtr:
    .word 0	# limit
    .quad 0	# base
