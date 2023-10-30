.section .text
.code64

.global set_CR3
set_CR3:
	mov %rdi, %cr3
	ret

.global get_CR3
get_CR3:
	mov %cr3, %rax
	ret
