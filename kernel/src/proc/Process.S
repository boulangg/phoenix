.section .text
.code64

.global switch_to_user_space
switch_to_user_space:
	pop %rcx	// entry point
	pushfq
	pop %r11	// rflags
	sysretq
