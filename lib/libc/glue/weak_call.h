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

int sys_open(const char *pathname, int flags, mode_t mode) __attribute__((weak));
int sys_close(int fd) __attribute__((weak));
ssize_t sys_read(int fd, void *buf, size_t count) __attribute__((weak));
ssize_t sys_write(int fd, const void *buf, size_t count) __attribute__((weak));
off_t sys_llseek(unsigned int fd, off_t offset, unsigned int whence) __attribute__((weak));

int sys_fork() __attribute__((weak));
int sys_execve(const char *file, char *const argv[], char *const envp[]) __attribute__((weak));
int sys_getpid() __attribute__((weak));
void sys_exit(int status) __attribute__((weak));

void* sys_brk(void* addr) __attribute__((weak));

int sys_nanosleep(const struct timespec *req, struct timespec *rem) __attribute__((weak));

int sys_getdents64(unsigned int fd, struct linux_dirent64 *dirp, unsigned int count) __attribute__((weak));

#endif // __WEAK_CALL_H__
