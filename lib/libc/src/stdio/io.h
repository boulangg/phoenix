/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
#pragma once

#include <fcntl.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

// Flags
#define MAGIC_MASK  0xFFFF0000
#define MAGIC_VALUE 0xABCD0000

#define SF_READ    0x00000001
#define SF_WRITE   0x00000002
#define SF_STRING  0x00000010
#define SF_USERBUF 0x00000020

typedef struct FILE
{
    uint64_t flags;
    // Buffer for read/write
    char* bufStart;
    char* bufPos;
    char* bufEnd;
    size_t bufSize;
    // Buffer for unget characters
    char bufUnget;
    // File info
    int64_t fileno;
    int64_t mode;
    bool eof;
    bool error;
} FILE;

#define CHECK_FILE(str)                                                                                                \
    {                                                                                                                  \
        if ((str->flags & MAGIC_MASK) != MAGIC_VALUE)                                                                  \
            return EOF;                                                                                                \
    }

void init_io();
void init_file(FILE* str);
