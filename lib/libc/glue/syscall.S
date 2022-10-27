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
SYSCALL(8, sys_lseek)
SYSCALL(9, sys_mmap)
SYSCALL(10, sys_protect)
SYSCALL(11, sys_munmap)
SYSCALL(12, sys_brk)
SYSCALL(26, sys_msync)
SYSCALL(35, sys_nanosleep)
SYSCALL(39, sys_getpid)
SYSCALL(57, sys_fork)
SYSCALL(59, sys_execve)
SYSCALL(60, sys_exit)
SYSCALL(62, sys_llseek)
SYSCALL(79, sys_getcwd)
SYSCALL(80, sys_chdir)
SYSCALL(81, sys_fchdir)
SYSCALL(149, sys_mlock)
SYSCALL(150, sys_munlock)
SYSCALL(151, sys_mlockall)
SYSCALL(152, sys_munlockall)
SYSCALL(201, sys_time)
SYSCALL(227, sys_clock_settime)
SYSCALL(228, sys_clock_gettime)
SYSCALL(229, sys_clock_getres)
SYSCALL(217, sys_getdents64)
