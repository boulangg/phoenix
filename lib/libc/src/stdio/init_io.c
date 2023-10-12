/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "io.h"

#include <stddef.h>
#include <stdlib.h>

void fini_io()
{
    fclose(stdout);
    fclose(stdin);
    fclose(stderr);
}

void init_file(FILE* str)
{
    str->flags = MAGIC_VALUE;
    str->bufStart = NULL;
    str->bufPos = NULL;
    str->bufEnd = NULL;
    str->bufUnget = EOF;
    str->eof = false;
    str->error = false;
}

void init_io()
{
    // 0: stdin
    stdin = malloc(sizeof(FILE));
    stdin->fileno = 0;
    init_file(stdin);
    setvbuf(stdin, NULL, _IOLBF, BUFSIZ);
    stdin->eof = false;
    stdin->error = false;
    // TODO finish to properly setup stdin

    // 1: stdout
    stdout = malloc(sizeof(FILE));
    stdout->fileno = 1;
    init_file(stdout);
    setvbuf(stdout, NULL, _IOLBF, BUFSIZ);

    // 2: stderr
    stderr = malloc(sizeof(FILE));
    stderr->fileno = 2;
    init_file(stderr);
    setvbuf(stderr, NULL, _IOLBF, BUFSIZ);

    atexit(fini_io);
}
