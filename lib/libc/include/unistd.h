/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef __UNISTD__
#define __UNISTD__


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
int close(int fd);
int read(int fd, void *buf, size_t count);
int write(int fd, const void *buf, size_t count);
off_t lseek(int fd, off_t offset, int whence);

int fork();
int execve(const char *file, char *const argv[], char *const envp[]);
int getpid();

int brk(void* addr);
void* sbrk(intptr_t increment);

unsigned int sleep(unsigned int seconds);

char *getcwd(char *buf, size_t size);
int chdir(const char *path);
int fchdir(int fd);

#ifdef __cplusplus
}
#endif

#endif
