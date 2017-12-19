#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include <fcntl.h>
#include <sys/types.h>

#include <linux_syscall.h>

/*struct linux_dirent {
	unsigned long d_ino;
	unsigned long d_off;
	unsigned short d_reclen;
	char           d_name[1];
};*/

#ifdef __cplusplus
extern "C" {
#endif

int sys_open(const char *pathname, int flags, mode_t mode);
int sys_close(unsigned int fd);
ssize_t sys_read(unsigned int fd, void *buf, size_t count);
ssize_t sys_write(unsigned int fd, const void *buf, size_t count);
off_t sys_llseek(unsigned int fd, off_t offset, unsigned int whence);

int sys_fork();
int sys_execve(const char *file, char *const argv[], char *const envp[]);
int sys_getpid();

void* sys_brk(void* addr);

int sys_nanosleep(const struct timespec *req, struct timespec *rem);

//int sys_getdents(unsigned int fd, struct linux_dirent *dirp, unsigned int count);
int sys_getdents64(unsigned int fd, struct linux_dirent64 *dirp, unsigned int count);

#ifdef __cplusplus
}
#endif

#endif // __SYSCALL_H__
