/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <unistd.h>

#include <syscall/syscall.h>

char* getcwd(char* buf, size_t size)
{
    return sys_getcwd(buf, size);
}

int chdir(const char* path)
{
    return sys_chdir(path);
}

int fchdir(int fd)
{
    return sys_fchdir(fd);
}
