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

	ssize_t sys_read(unsigned int fd, void* buf, size_t count);
	ssize_t sys_write(unsigned int fd, const void* buf, size_t count);
	off_t sys_lseek(unsigned int fd, off_t offset, int whence);
	int sys_open(const char* pathname, int flags, mode_t mode);
	int sys_close(unsigned int fd);
	int sys_stat(const char*, struct stat*);
	int sys_fstat(int, struct stat*);
	int sys_lstat(const char*, struct stat*);
	off_t sys_llseek(unsigned int fd, off_t offset, unsigned int whence);

	int sys_fork();
	int sys_execve(const char* file, char* const argv[], char* const envp[]);
	int sys_getpid();

	void* sys_brk(void* addr);

	int sys_nanosleep(const struct timespec* req, struct timespec* rem);

	//int sys_getdents(unsigned int fd, struct linux_dirent *dirp, unsigned int count);
	int sys_getdents64(unsigned int fd, struct linux_dirent64* dirp, unsigned int count);

	char* sys_getcwd(char* buf, size_t size);
	int sys_chdir(const char* path);
	int sys_fchdir(int fd);

	void sys_exit(int returnCode);

	int sys_mlock(const void* addr, size_t length);
	int sys_mlockall(int flags);
	int sys_munlock(const void* addr, size_t length);
	int sys_munlockall(void);

	void* sys_mmap(void* addr, size_t length, int prot, int flags, int fd, off_t offset);
	int sys_munmap(void* addr, size_t length);

	int sys_mprotect(void* addr, size_t length, int prot);
	int sys_msync(void* addr, size_t length, int flags);

#ifdef __cplusplus
}
#endif

#endif // __SYSCALL_H__
