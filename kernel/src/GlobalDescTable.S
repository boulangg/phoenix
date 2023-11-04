.section .text
.code64

.global set_GDT
set_GDT:
	mov %edi, gdtr
	mov %rsi, gdtr+2
	lgdt gdtr
	ret

.global set_TSS
set_TSS:
	mov %edi, tr
	ltr tr
	ret

.section .data

.align 8
gdtr:
    .word 0	# limit
    .quad 0	# base

.align 8
tr:
	.word 0x00	# selector
