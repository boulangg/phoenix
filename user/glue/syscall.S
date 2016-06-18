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

SYSCALL(0, sys_read)
SYSCALL(1, sys_write)
SYSCALL(2, sys_open)
SYSCALL(3, sys_close)
SYSCALL(39, sys_getpid)
SYSCALL(57, sys_fork)
SYSCALL(59, sys_execve)
SYSCALL(60, sys_exit)

