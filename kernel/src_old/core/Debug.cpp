/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "Debug.hpp"

#include <cstdio>
#include <stdarg.h>

#include <core/Console.hpp>

#define BUFFER_SIZE 1024

void printk(const char* format, ...)
{
    char tmp[BUFFER_SIZE];
    unsigned long int size = BUFFER_SIZE;
    va_list vl;
    vsnprintf(tmp, size, format, vl);
    Console::write(tmp);
}
