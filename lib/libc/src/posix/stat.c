/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <sys/stat.h>

#include <syscall/syscall.h>

int stat(const char* pathname, struct stat* stat)
{
    return sys_stat(pathname, stat);
}

int fstat(int fd, struct stat* stat)
{
    return sys_fstat(fd, stat);
}

int lstat(const char* pathname, struct stat* stat)
{
    return sys_lstat(pathname, stat);
}
