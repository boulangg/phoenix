/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "io.h"

#include <stdlib.h>
#include <errno.h>

FILE * fopen(const char* filename, const char* mode) {
	int flags = 0;
	//int file_flags = 0;
	switch(mode[0]) {
	case 'r':
		if (mode[1] == '+') {
			flags |= O_RDWR;
		} else {
			flags |= O_RDONLY;
		}
		break;
	case 'w':
		flags |= O_TRUNC | O_CREAT;
		if (mode[1] == '+') {
			flags |= O_RDWR;
		} else {
			flags |= O_WRONLY;
		}
		break;
	case 'a':
		flags |= O_CREAT;
		if (mode[1] == '+') {
			flags |= O_RDWR;
		} else {
			flags |= O_WRONLY;
		}
		break;
	default:
		return NULL;
	}
	int fileMode = 0x0777;

	int fd = open(filename, flags, fileMode);
	if (fd < 0) {
		errno = fd;
		return NULL;
	}
	FILE* f = malloc(sizeof(FILE));
	f->fileno = fd;
	f->flags = MAGIC_VALUE;
	f->mode = _IONBF;		// TODO change to full buffer or line buffer
	f->offset = 0;
	setvbuf(f, NULL, _IONBF, BUFSIZ);
	f->eof = false;
	f->error = 0;
	return f;
}
