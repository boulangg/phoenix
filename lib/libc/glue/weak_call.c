/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "weak_call.h"

void *sbrk(ptrdiff_t increment) {
	(void)increment;
	return 0;
}

void sys_exit(int status) {
	(void)status;
	__builtin_unreachable();
}

int sys_write(int fd, void* buf, size_t count) {
	(void)fd;
	(void)buf;
	(void)count;
	return -1;
}

int sys_read(int fd, void* buf, size_t count) {
	(void)fd;
	(void)buf;
	(void)count;
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
