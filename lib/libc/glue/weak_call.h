/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef __WEAK_CALL_H__
#define __WEAK_CALL_H__

#include <stddef.h>
#include <time.h>

int sys_open(const char *pathname, int flags, mode_t mode) __attribute__((weak));
int sys_close(int fd) __attribute__((weak));
ssize_t sys_read(int fd, void *buf, size_t count) __attribute__((weak));
ssize_t sys_write(int fd, const void *buf, size_t count) __attribute__((weak));

int sys_fork() __attribute__((weak));
int sys_execve(const char *file, char *const argv[], char *const envp[]) __attribute__((weak));
int sys_getpid() __attribute__((weak));
void sys_exit(int status) __attribute__((weak));

void* sys_brk(void* addr) __attribute__((weak));

int sys_nanosleep(const struct timespec *req, struct timespec *rem) __attribute__((weak));

#endif // __WEAK_CALL_H__
