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
SYSCALL(4, sys_stat)
SYSCALL(5, sys_fstat)
SYSCALL(6, sys_lstat)
SYSCALL(12, sys_brk)
SYSCALL(35, sys_nanosleep)
SYSCALL(39, sys_getpid)
SYSCALL(57, sys_fork)
SYSCALL(59, sys_execve)
SYSCALL(60, sys_exit)
SYSCALL(79, sys_getcwd)
SYSCALL(80, sys_chdir)
SYSCALL(81, sys_fchdir)
SYSCALL(217, sys_getdents64)

