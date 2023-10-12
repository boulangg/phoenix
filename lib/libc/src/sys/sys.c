/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "sys.h"

#include "sys/auxv.h"
#include <stddef.h>

char** environ;
auxv_t* __auxv_ptr;

void init_sys(char** envp)
{
    environ = envp;

    size_t i = 0;
    while (environ[i] != 0) {
        i++;
    }

    __auxv_ptr = (auxv_t*)(envp + i + 1);
}
