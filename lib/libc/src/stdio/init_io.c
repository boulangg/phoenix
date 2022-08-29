/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "io.h"

#include <stdlib.h>
#include <stddef.h>

FILE* stdout;
FILE* stdin;
FILE* stderr;

void init_io()
{
// 0: stdin
	stdin = malloc(sizeof(FILE));
	stdin->fileno = 0;
	stdin->flags = MAGIC_VALUE;
	stdin->fn = &file_fn;
	stdin->bufStart = NULL;
	//stdin->mode = _IONBF;		// TODO Change to line buffer (or full buffer)
	setvbuf(stdin, NULL, _IONBF, BUFSIZ);
	stdin->eof = false;
	stdin->bufVirtPos = BUF_VIRT_SIZE;
	// TODO finish to properly setup stdin

	// 1: stdout
	stdout = malloc(sizeof(FILE));
	stdout->fileno = 1;
	stdout->flags = MAGIC_VALUE;
	stdout->fn = &file_fn;
	stdout->bufStart = NULL;
	//stdout->mode = _IONBF;		// TODO Change to line buffer (or full buffer)
	setvbuf(stdout, NULL, _IONBF, BUFSIZ);
	stdout->eof = false;
	stdout->bufVirtPos = BUF_VIRT_SIZE;
	// TODO finish to properly setup stdin

	// 12 stderr
	stderr = malloc(sizeof(FILE));
	stderr->fileno = 2;
	stderr->flags = MAGIC_VALUE;
	stderr->fn = &file_fn;
	stderr->bufStart = NULL;
	//stderr->mode = _IONBF;		// TODO Change to line buffer (or full buffer)
	setvbuf(stderr, NULL, _IONBF, BUFSIZ);
	stderr->eof = false;
	stderr->bufVirtPos = BUF_VIRT_SIZE;
	// TODO finish to properly setup stdout
}

void bufToFile(FILE* str, char* s, size_t n)
{
	str->flags = MAGIC_VALUE;
	str->bufStart = s;
	str->bufPos = s;
	str->bufEnd = s;
	str->bufSize = n;
	// Check overflow
	if (str->bufStart + str->bufSize < str->bufStart) {
		str->bufSize = (size_t)((char*)-1 - str->bufStart);
	}
	str->bufVirtPos = BUF_VIRT_SIZE;
	str->eof = false;
	str->fn = &str_fn;
}
