/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "Syscall.hpp"

#include <errno.h>

#include <core/Clock.hpp>
#include <fs/vfs/VirtualFileSystem.hpp>
#include <proc/ProcessScheduler.hpp>

uint64_t syscall64(uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e, uint64_t f, uint64_t num)
{
    (void)a;
    (void)b;
    (void)c;
    (void)d;
    (void)e;
    (void)f;
    return syscall64_table[num](a, b, c, d, e, f);
}

uint64_t syscall64_empty(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t)
{
    return ENOSYS;
}

uint64_t syscall64_read(uint64_t a, uint64_t b, uint64_t c, uint64_t, uint64_t, uint64_t)
{
    unsigned int fd = (unsigned int)a;
    char* buffer = (char*)b;
    size_t size = (size_t)c;
    File* file = ProcessScheduler::getFile(fd);
    if (file == nullptr) {
        return ENOENT;
    }
    return file->read(buffer, size);
}

uint64_t syscall64_write(uint64_t a, uint64_t b, uint64_t c, uint64_t, uint64_t, uint64_t)
{
    unsigned int fd = (unsigned int)a;
    char* buffer = (char*)b;
    size_t size = (size_t)c;
    File* file = ProcessScheduler::getFile(fd);
    if (file == nullptr) {
        return ENOENT;
    }
    return file->write(buffer, size);
}

uint64_t syscall64_open(uint64_t a, uint64_t b, uint64_t c, uint64_t, uint64_t, uint64_t)
{
    const char* pathname = (const char*)a;
    int flags = (int)b;
    mode_t mode = (mode_t)c;
    return ProcessScheduler::open(pathname, flags, mode);
}

uint64_t syscall64_brk(uint64_t a, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t)
{
    void* addr = (void*)a;
    return (uint64_t)ProcessScheduler::userBrk(addr);
}

uint64_t syscall64_nanosleep(uint64_t a, uint64_t b, uint64_t, uint64_t, uint64_t, uint64_t)
{
    const timespec* req = (const timespec*)a;
    timespec* rem = (timespec*)b;
    return Clock::nanosleep(req, rem);
}

uint64_t syscall64_getpid(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t)
{
    return ProcessScheduler::getpid();
}

uint64_t syscall64_fork(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t)
{
    return ProcessScheduler::fork();
}

uint64_t syscall64_execve(uint64_t a, uint64_t b, uint64_t c, uint64_t, uint64_t, uint64_t)
{
    const char* filename = (const char*)a;
    const char** argv = (const char**)b;
    const char** envp = (const char**)c;
    return ProcessScheduler::execve(filename, argv, envp);
}

uint64_t syscall64_exit(uint64_t a, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t)
{
    int status = (uint64_t)a;
    ProcessScheduler::exit(status);
    // should never return
    return ENOSYS;
}

uint64_t syscall64_getdents64(uint64_t a, uint64_t b, uint64_t c, uint64_t, uint64_t, uint64_t)
{
    unsigned int fd = (unsigned int)a;
    struct linux_dirent64* dirp = (struct linux_dirent64*)b;
    size_t size = (size_t)c;
    File* file = ProcessScheduler::getFile(fd);
    if (file == nullptr) {
        return ENOENT;
    }
    return file->getdents64(dirp, size);
}

uint64_t syscall64_getcwd(uint64_t a, uint64_t b, uint64_t, uint64_t, uint64_t, uint64_t)
{
    char* buffer = (char*)a;
    size_t size = (size_t)b;
    return (uint64_t)ProcessScheduler::getcwd(buffer, size);
}

uint64_t syscall64_chdir(uint64_t a, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t)
{
    const char* buffer = (const char*)a;
    return ProcessScheduler::chdir(buffer);
}

uint64_t syscall64_fchdir(uint64_t a, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t)
{
    unsigned int fd = (unsigned int)a;
    return ProcessScheduler::fchdir(fd);
}

uint64_t syscall64_stat(uint64_t a, uint64_t b, uint64_t, uint64_t, uint64_t, uint64_t)
{
    const char* path = (const char*)a;
    struct stat* buf = (struct stat*)b;
    return VirtualFileSystem::stat(path, buf);
}

uint64_t syscall64_fstat(uint64_t a, uint64_t b, uint64_t, uint64_t, uint64_t, uint64_t)
{
    unsigned int fd = (unsigned int)a;
    struct stat* buf = (struct stat*)b;
    File* file = ProcessScheduler::getFile(fd);
    if (file == nullptr) {
        return ENOENT;
    }
    return VirtualFileSystem::fstat(file, buf);
}

uint64_t syscall64_lstat(uint64_t a, uint64_t b, uint64_t, uint64_t, uint64_t, uint64_t)
{
    const char* path = (const char*)a;
    struct stat* buf = (struct stat*)b;
    return VirtualFileSystem::lstat(path, buf);
}

syscall64_t syscall64_table[256] = {
    // 0-31
    syscall64_read,
    syscall64_write,
    syscall64_open,
    syscall64_empty,
    syscall64_stat,
    syscall64_fstat,
    syscall64_lstat,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_brk,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    // 32-63
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_nanosleep,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_getpid,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_fork,
    syscall64_empty,
    syscall64_execve,
    syscall64_exit,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    // 64-95
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_getcwd,
    syscall64_chdir,
    syscall64_fchdir,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    // 96-127
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    // 128-159
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    // 160-191
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    // 192-223
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_getdents64,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    // 224-255
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
    syscall64_empty,
};
