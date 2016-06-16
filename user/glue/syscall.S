.text

#define SYSCALL(number, fn) \
	.global fn;				\
	fn:						\
		push %rbp;			\
		mov %rsp, %rbp;		\
		mov %rcx, %r10;		\
		mov $number, %rax;	\
		syscall;			\
		pop %rbp;			\
		ret

SYSCALL(60, sys_exit)
SYSCALL(57, sys_fork)
SYSCALL(59, sys_execve)
SYSCALL(39, sys_getpid)

