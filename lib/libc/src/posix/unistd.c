/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <time.h>
#include <unistd.h>

#include <syscall/syscall.h>

int close(int fd)
{
    return sys_close(fd);
}

ssize_t read(int fd, void* buf, size_t count)
{
    return sys_read(fd, buf, count);
}

ssize_t write(int fd, const void* buf, size_t count)
{
    return sys_write(fd, buf, count);
}

off_t lseek(int fd, off_t offset, int whence)
{
    return sys_lseek(fd, offset, whence);
}

void _exit(int status)
{
    sys_exit(status);
    __builtin_unreachable();
}

int fork()
{
    return sys_fork();
}

int execve(const char* file, char* const argv[], char* const envp[])
{
    return sys_execve(file, argv, envp);
}

int getpid()
{
    return sys_getpid();
}

int brk(void* addr)
{
    void* oldBrk = sys_brk((void*)0);
    if (sys_brk(addr) != oldBrk) {
        return 0;
    } else {
        // errno = ENOMEM;
        return -1;
    }
}

unsigned int sleep(unsigned int seconds)
{
    struct timespec spec;
    spec.tv_nsec = 0;
    spec.tv_sec = seconds;
    unsigned int ret = nanosleep(&spec, NULL);
    return ret;
}

void* sbrk(intptr_t increment)
{
    if (increment == 0) {
        return sys_brk(0);
    }
    char* oldBrk = sys_brk(0);
    if ((char*)sys_brk((void*)(oldBrk + increment)) != oldBrk) {
        return (void*)oldBrk;
    } else {
        // errno = ENOMEM;
        return (void*)-1;
    }
}
