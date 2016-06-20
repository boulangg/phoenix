#include <unistd.h>

int open(const char *pathname, int flags, mode_t mode) {
	return sys_open(pathname, flags, mode);
}

int close(int fd) {
	return sys_close(fd);
}

int read(int fd, void *buf, size_t count) {
	return sys_read(fd, buf, count);
}

int write(int fd, const void *buf, size_t count) {
	return sys_write(fd, buf, count);
}

int fork() {
	return sys_fork();
}

int execve(const char *file, char *const argv[], char *const envp[]) {
	return sys_execve(file, argv, envp);
}

int getpid() {
	return sys_getpid();
}

int brk(void* addr) {
    int oldBrk = sys_brk(0);
	if (sys_brk((void*)(addr)) != oldBrk) {
		return 0;
	} else {
		//errno = ENOMEM;
		return -1;
	}
	return sys_brk(addr);
}

void* sbrk(intptr_t increment) {
	int oldBrk = sys_brk(0);
	if (sys_brk((void*)(oldBrk + increment)) != oldBrk) {
		return (void*)(intptr_t)oldBrk;
	} else {
		//errno = ENOMEM;
		return (void*)-1;
	}
}
