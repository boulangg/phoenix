/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include "cstdlib"

#include <stdlib.h>

namespace std {

void exit(int exit_code)
{
    return ::exit(exit_code);
}

void* malloc(std::size_t size)
{
    return ::malloc(size);
}

void free(void* ptr)
{
    return ::free(ptr);
}

}
