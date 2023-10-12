/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "stdlib.h"

#include "stdio.h"
#include "syscall/syscall.h"

void abort(void)
{
    printf("Aborted\n");
    sys_exit(-1);
    __builtin_unreachable();
}
