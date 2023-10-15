/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <time.h>

#include <syscall/syscall.h>

int nanosleep(const struct timespec* req, struct timespec* rem)
{
    return sys_nanosleep(req, rem);
}
