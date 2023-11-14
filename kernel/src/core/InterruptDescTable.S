.section .text
.code64

.global set_IDT
set_IDT:
	mov %edi, idtr
	mov %rsi, idtr+2
	lidt idtr
	ret

.section .data

.align 8
idtr:
    .word 0	# limit
    .quad 0	# base
