/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "elf64.h"

size_t elf64_hash(const unsigned char* name)
{
    size_t h = 0, g;
    while (*name) {
        h = (h << 4) + *name++;
        if ((g = h & 0xf0000000) != 0) {
            h ^= g >> 24;
        }
        h &= 0x0fffffff;
    }
    return h;
}
