/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <stdio.h>

int fprintf(FILE* str, const char* format, ...)
{
    va_list v;
    va_start(v, format);
    int res = vfprintf(str, format, v);
    va_end(v);
    return res;
}

int fscanf(FILE* str, const char* format, ...)
{
    va_list v;
    va_start(v, format);
    int res = vfscanf(str, format, v);
    va_end(v);
    return res;
}
