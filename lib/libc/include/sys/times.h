/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <sys/types.h>

struct timeval
{
    time_t tv_sec;       /* seconds */
    suseconds_t tv_usec; /* microseconds */
};

struct tms
{
    clock_t tms_utime;  // user CPU time
    clock_t tms_stime;  // system CPU time
    clock_t tms_cutime; // user CPU of terminated child processes
    clock_t tms_cstime; // system CPU time of terminated child processes
};

clock_t times(struct tms*);
