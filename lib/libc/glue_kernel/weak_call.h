/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef __WEAK_CALL_H__
#define __WEAK_CALL_H__

#include <fcntl.h>
#include <stddef.h>
#include <time.h>
#include <linux_syscall.h>


struct linux_dirent {
	unsigned long  d_ino;     /* Inode number */
	unsigned long  d_off;     /* Offset to next linux_dirent */
	unsigned short d_reclen;  /* Length of this linux_dirent */
	char           d_name[1]; /* Filename (null-terminated) */
};

#define WEAK_SYSCALL __attribute__((weak))

int sys_open(const char *pathname, int flags, mode_t mode) WEAK_SYSCALL;
int sys_close(int fd) WEAK_SYSCALL;
ssize_t sys_read(int fd, void *buf, size_t count) WEAK_SYSCALL;
ssize_t sys_write(int fd, const void *buf, size_t count) WEAK_SYSCALL;
int sys_stat(const char*, struct stat*) WEAK_SYSCALL;
int sys_fstat(int, struct stat*) WEAK_SYSCALL;
int sys_lstat(const char*, struct stat*) WEAK_SYSCALL;
off_t sys_llseek(unsigned int fd, off_t offset, unsigned int whence) WEAK_SYSCALL;

int sys_fork() WEAK_SYSCALL;
int sys_execve(const char *file, char *const argv[], char *const envp[]) WEAK_SYSCALL;
int sys_getpid() WEAK_SYSCALL;
void sys_exit(int status) WEAK_SYSCALL;

void* sys_brk(void* addr) WEAK_SYSCALL;

int sys_nanosleep(const struct timespec *req, struct timespec *rem) WEAK_SYSCALL;

int sys_getdents64(unsigned int fd, struct linux_dirent64 *dirp, unsigned int count) WEAK_SYSCALL;

char *sys_getcwd(char *buf, size_t size) WEAK_SYSCALL;
int sys_chdir(const char *path) WEAK_SYSCALL;
int sys_fchdir(int fd) WEAK_SYSCALL;

int sys_mlock(const void* addr, size_t length) WEAK_SYSCALL;
int sys_mlockall(int flags) WEAK_SYSCALL;
int sys_munlock(const void* addr, size_t length) WEAK_SYSCALL;
int sys_munlockall(void) WEAK_SYSCALL;

void* sys_mmap(void* addr, size_t length, int prot, int flags, int fd, off_t offset) WEAK_SYSCALL;
int sys_munmap(void* addr, size_t length) WEAK_SYSCALL;

int sys_mprotect(void* addr, size_t length, int prot) WEAK_SYSCALL;
int sys_msync(void* addr, size_t length, int flags) WEAK_SYSCALL;

#endif // __WEAK_CALL_H__
