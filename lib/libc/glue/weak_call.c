/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "weak_call.h"

int sys_open(const char *pathname, int flags, mode_t mode) {
	(void)pathname;
	(void)flags;
	(void)mode;
	return -1;
}

ssize_t sys_write(int fd, const void *buf, size_t count) {
	(void)fd;
	(void)buf;
	(void)count;
	return -1;
}

ssize_t sys_read(int fd, void *buf, size_t count) {
	(void)fd;
	(void)buf;
	(void)count;
	return -1;
}

int sys_close(int fd) {
	(void)fd;
	return 0;
}


int sys_stat(const char* pathname, struct stat* stat) {
	(void)pathname;
	(void)stat;
	return -1;
}

int sys_fstat(int fd, struct stat* stat) {
	(void)fd;
	(void)stat;
	return -1;
}

int sys_lstat(const char* pathname, struct stat* stat) {
	(void)pathname;
	(void)stat;
	return -1;
}

off_t sys_llseek(unsigned int fd, off_t offset, unsigned int whence) {
	(void)fd;
	(void)offset;
	(void)whence;
	return -1;
}

int sys_fork() {
	return -1;
}

int sys_execve(const char *file, char *const argv[], char *const envp[]) {
	(void)file;
	(void)argv;
	(void)envp;
	return -1;
}

int sys_getpid() {
	return 1;
}

void sys_exit(int status) {
	(void)status;
	__builtin_unreachable();
}

void* sys_brk(void* addr) {
	(void)addr;
	return 0;
}

int sys_nanosleep(const struct timespec *req, struct timespec *rem) {
	(void)req;
	(void)rem;
	return -1;
}

int sys_getdents64(unsigned int fd, struct linux_dirent64 *dirp, unsigned int count) {
	(void)fd;
	(void)dirp;
	(void)count;
	return 0;
}

char *sys_getcwd(char *buf, size_t size) {
	(void)buf;
	(void)size;
	return 0;
}

int sys_chdir(const char *path) {
	(void)path;
	return -1;
}

int sys_fchdir(int fd) {
	(void)fd;
	return -1;
}

