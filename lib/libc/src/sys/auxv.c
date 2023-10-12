/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "sys/auxv.h"

#include <stddef.h>

extern auxv_t* __auxv_ptr;

uint64_t getauxval(uint64_t __type)
{
    size_t i = 0;
    while (__auxv_ptr[i].a_type != AT_NULL) {
        if (__auxv_ptr[i].a_type == __type) {
            return __auxv_ptr[i].a_un.a_val;
        }
        i++;
    }
    return 0;
}
