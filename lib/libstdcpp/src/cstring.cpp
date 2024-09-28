/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "cstring"

#include <string.h>

namespace std {

int strcmp(const char* lhs, const char* rhs)
{
    return ::strcmp(lhs, rhs);
}

char* strcpy(char* dest, const char* src)
{
    return ::strcpy(dest, src);
}

char* strncpy(char* dest, const char* src, std::size_t num)
{
    return ::strncpy(dest, src, num);
}

std::size_t strlen(const char* str)
{
    return ::strlen(str);
}

void* memcpy(void* dest, const void* src, std::size_t count)
{
    return ::memcpy(dest, src, count);
}

}
