#ifndef __UNISTD__
#define __UNISTD__

#define O_RDONLY  0x0000
#define O_WRONLY  0x0001
#define O_RDWR    0x0002

#define O_CREAT   0x0200
#define O_TRUNC   0x0400

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <def/def_size.h>

typedef int mode_t;
typedef int ssize_t;

// Syscall (don't use it directly)
int sys_open(const char *pathname, int flags, mode_t mode);
int sys_close(int fd);
ssize_t sys_read(int fd, void *buf, size_t count);
ssize_t sys_write(int fd, const void *buf, size_t count);

int sys_fork();
int sys_execve(const char *file, char *const argv[], char *const envp[]);
int sys_getpid();

int sys_brk(void* addr);

// POSIX functions
int open(const char *pathname, int flags, mode_t mode) { return sys_open(pathname, flags, mode); }
int close(int fd) { return sys_close(fd); }
int read(int fd, void *buf, size_t count) { return sys_read(fd, buf, count); }
int write(int fd, const void *buf, size_t count) { return sys_write(fd, buf, count); }

int fork() { return sys_fork(); }
int execve(const char *file, char *const argv[], char *const envp[]) { return sys_execve(file, argv, envp); }
int getpid() { return sys_getpid(); }

/*int brk(void* addr) {
	return sys_brk(increment);
}*/
void* sbrk(intptr_t increment) __attribute__((weak));
void* sbrk(intptr_t increment) {
	int oldBrk = sys_brk(0);
	if (sys_brk((void*)(oldBrk + increment)) != oldBrk) {
		return (void*)(intptr_t)oldBrk;
	} else {
		//errno = ENOMEM;
		return (void*)-1;
	}
}

#ifdef __cplusplus
}
#endif

#endif
