/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <syscall/syscall.h>
#include <time.h>

time_t time(time_t* tloc)
{
    return sys_time(tloc);
}

int clock_getres(clockid_t clk_id, struct timespec* res)
{
    return sys_clock_getres(clk_id, res);
}

int clock_gettime(clockid_t clk_id, struct timespec* tp)
{
    return sys_clock_gettime(clk_id, tp);
}

int clock_settime(clockid_t clk_id, const struct timespec* tp)
{
    return sys_clock_settime(clk_id, tp);
}
