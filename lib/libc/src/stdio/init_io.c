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

void init_io() {
	stdout = malloc(sizeof(FILE));
	stdout->fileno = 1;
	stdout->flags = MAGIC_VALUE;
	stdout->fn = &file_fn;
	//stdout->mode = _IONBF;		// TODO Change to line buffer (or full buffer)
	setvbuf(stdout, NULL, _IONBF, BUFSIZ);
	stdout->eof = false;
	stdout->bufVirtPos = BUF_VIRT_SIZE;
	// TODO init stdin and stderr, and finish to properly setup stdout
}

void bufToFile(FILE* str, char* s, size_t n) {
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
