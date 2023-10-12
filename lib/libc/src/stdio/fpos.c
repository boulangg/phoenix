/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "io.h"

int fgetpos(FILE* str, fpos_t* pos)
{
    CHECK_FILE(str);
    *pos = ftell(str);
    return 0;
}

int fseek(FILE* str, long int offset, int origin)
{
    CHECK_FILE(str);
    fflush(str);
    str->bufPos = str->bufEnd;
    return lseek(str->fileno, offset, origin);
}

int fsetpos(FILE* str, const fpos_t* pos)
{
    CHECK_FILE(str);
    return fseek(str, *pos, SEEK_SET);
}

long int ftell(FILE* str)
{
    CHECK_FILE(str);
    return lseek(str->fileno, 0, SEEK_CUR) - (str->bufEnd - str->bufPos);
}

void rewind(FILE* str)
{
    fseek(str, 0, SEEK_SET);
}
