/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <stdio.h>

#include "io.h"

void bufToFile(FILE* str, char* s, size_t n)
{
    init_file(str);
    str->flags |= SF_STRING;
    str->bufStart = s;
    str->bufPos = s;
    str->bufSize = n;
    // Check overflow
    if (str->bufStart + str->bufSize < str->bufStart) {
        str->bufSize = (size_t)((char*)-1 - str->bufStart);
    }
    str->bufEnd = str->bufStart + str->bufSize;
}

int vsscanf(const char* s, const char* format, va_list arg)
{
    FILE str;
    bufToFile(&str, (char*)s, (size_t)-1);
    int res = vfscanf(&str, format, arg);
    return res;
}

int vsprintf(char* s, const char* format, va_list arg)
{
    FILE str;
    bufToFile(&str, s, (size_t)-1);
    int res = vfprintf(&str, format, arg);
    if (res > 0) {
        s[res] = '\0';
    }
    return res;
}

int vsnprintf(char* s, size_t n, const char* format, va_list arg)
{
    if (n == 0) {
        return -1;
    }
    FILE str;
    bufToFile(&str, s, n - 1);
    int res = vfprintf(&str, format, arg);
    if (res >= 0) {
        s[res] = '\0';
    }
    return res;
}

int sscanf(const char* s, const char* format, ...)
{
    va_list v;
    va_start(v, format);
    FILE str;
    bufToFile(&str, (char*)s, (size_t)-1);
    int res = vfscanf(&str, format, v);
    va_end(v);
    return res;
}

int sprintf(char* s, const char* format, ...)
{
    va_list v;
    va_start(v, format);
    FILE str;
    bufToFile(&str, s, (size_t)-1);
    int res = vfprintf(&str, format, v);
    if (res >= 0) {
        s[res] = '\0';
    }
    va_end(v);
    return res;
}

int snprintf(char* s, size_t n, const char* format, ...)
{
    if (n == 0) {
        return -1;
    }
    va_list v;
    va_start(v, format);
    FILE str;
    bufToFile(&str, s, n - 1);
    int res = vfprintf(&str, format, v);
    if (res >= 0) {
        s[res] = '\0';
    }
    va_end(v);
    return res;
}
