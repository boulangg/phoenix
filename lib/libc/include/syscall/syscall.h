#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

int sys_open(const char *pathname, int flags, mode_t mode);
int sys_close(int fd);
ssize_t sys_read(int fd, void *buf, size_t count);
ssize_t sys_write(int fd, const void *buf, size_t count);

int sys_fork();
int sys_execve(const char *file, char *const argv[], char *const envp[]);
int sys_getpid();

void* sys_brk(void* addr);

int sys_nanosleep(const struct timespec *req, struct timespec *rem);

#ifdef __cplusplus
}
#endif

#endif // __SYSCALL_H__
