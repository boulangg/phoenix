/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef __UNISTD__
#define __UNISTD__

#define O_RDONLY  0x0000
#define O_WRONLY  0x0001
#define O_RDWR    0x0002

#define O_CREAT   0x0200
#define O_TRUNC   0x0400

#include <stdint.h>
#include <def/def_size.h>
#include <sys/types.h>

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#ifdef __cplusplus
extern "C" {
#endif

// POSIX functions
int open(const char *pathname, int flags, mode_t mode);
int close(int fd);
int read(int fd, void *buf, size_t count);
int write(int fd, const void *buf, size_t count);

int fork();
int execve(const char *file, char *const argv[], char *const envp[]);
int getpid();

int brk(void* addr);
void* sbrk(intptr_t increment);

unsigned int sleep(unsigned int seconds);

#ifdef __cplusplus
}
#endif

#endif
