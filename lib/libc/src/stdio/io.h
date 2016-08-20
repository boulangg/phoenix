/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <stdio.h>
#include <stddef.h>

// Flags
#define MAGIC_MASK   0xFFFF0000
#define MAGIC_VALUE  0xABCD0000

#define FREAD        0x00000001
#define FWRITE       0x00000002
#define BUFWRITE     0x00000010
#define USERBUF      0x00000020

struct stream_ops str_fn;
struct stream_ops file_fn;

#define CHECK_FILE(str) {if (str == NULL) return EOF;                            \
                         if ((str->flags & MAGIC_MASK) != MAGIC_VALUE) return EOF; \
                         if (str->fn == NULL) return EOF;}

void init_io();

void bufToFile(FILE* str, char* s, size_t n);
