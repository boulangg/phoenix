/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "weak_call.h"

int sys_open(const char* pathname, int flags, mode_t mode)
{
    (void)pathname;
    (void)flags;
    (void)mode;
    return -1;
}

ssize_t sys_write(int fd, const void* buf, size_t count)
{
    (void)fd;
    (void)buf;
    (void)count;
    return -1;
}

ssize_t sys_read(int fd, void* buf, size_t count)
{
    (void)fd;
    (void)buf;
    (void)count;
    return -1;
}

off_t sys_lseek(unsigned int fd, off_t offset, int whence)
{
    (void)fd;
    (void)offset;
    (void)whence;
    return -1;
}

int sys_close(int fd)
{
    (void)fd;
    return 0;
}

int sys_stat(const char* pathname, struct stat* stat)
{
    (void)pathname;
    (void)stat;
    return -1;
}

int sys_fstat(int fd, struct stat* stat)
{
    (void)fd;
    (void)stat;
    return -1;
}

int sys_lstat(const char* pathname, struct stat* stat)
{
    (void)pathname;
    (void)stat;
    return -1;
}

off_t sys_llseek(unsigned int fd, off_t offset, unsigned int whence)
{
    (void)fd;
    (void)offset;
    (void)whence;
    return -1;
}

int sys_fork()
{
    return -1;
}

int sys_execve(const char* file, char* const argv[], char* const envp[])
{
    (void)file;
    (void)argv;
    (void)envp;
    return -1;
}

int sys_getpid()
{
    return 1;
}

void sys_exit(int status)
{
    (void)status;
    __builtin_unreachable();
}

void* sys_brk(void* addr)
{
    (void)addr;
    return 0;
}

int sys_nanosleep(const struct timespec* req, struct timespec* rem)
{
    (void)req;
    (void)rem;
    return -1;
}

int sys_getdents64(unsigned int fd, struct linux_dirent64* dirp, unsigned int count)
{
    (void)fd;
    (void)dirp;
    (void)count;
    return 0;
}

char* sys_getcwd(char* buf, size_t size)
{
    (void)buf;
    (void)size;
    return 0;
}

int sys_chdir(const char* path)
{
    (void)path;
    return -1;
}

int sys_fchdir(int fd)
{
    (void)fd;
    return -1;
}

int sys_mlock(const void* addr, size_t length)
{
    (void)addr;
    (void)length;
    return -1;
}

int sys_mlockall(int flags)
{
    (void)flags;
    return -1;
}

int sys_munlock(const void* addr, size_t length)
{
    (void)addr;
    (void)length;
    return -1;
}

int sys_munlockall(void)
{
    return -1;
}

void* sys_mmap(void* addr, size_t length, int prot, int flags, int fd, off_t offset)
{
    (void)addr;
    (void)length;
    (void)prot;
    (void)flags;
    (void)fd;
    (void)offset;
    return (void*)-1;
}

int sys_munmap(void* addr, size_t length)
{
    (void)addr;
    (void)length;
    return -1;
}

int sys_mprotect(void* addr, size_t length, int prot)
{
    (void)addr;
    (void)length;
    (void)prot;
    return -1;
}

int sys_msync(void* addr, size_t length, int flags)
{
    (void)addr;
    (void)length;
    (void)flags;
    return -1;
}

time_t sys_time(time_t* tloc)
{
    (void)tloc;
    return -1;
}

int sys_clock_getres(clockid_t clk_id, void* res)
{
    (void)clk_id;
    (void)res;
    return -1;
}

int sys_clock_gettime(clockid_t clk_id, void* tp)
{
    (void)clk_id;
    (void)tp;
    return -1;
}

int sys_clock_settime(clockid_t clk_id, const void* tp)
{
    (void)clk_id;
    (void)tp;
    return -1;
}
