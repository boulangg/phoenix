/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "io.h"
#include <unistd.h>


int _IO_str_fgetc(FILE* str)
{
	if (str->bufPos >= str->bufEnd) {
		return EOF;
	} else {
		return *(str->bufPos++);
	}
}

int _IO_str_fputc(int c, FILE* str)
{
	if (str->bufEnd >= str->bufStart + str->bufSize) {
		return EOF;
	} else {
		*(str->bufEnd++) = c;
		return c;
	}
}

struct stream_ops str_fn = {
		_IO_str_fgetc,
		_IO_str_fputc
};
