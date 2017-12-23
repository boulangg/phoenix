/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "io.h"

#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

#define MODE_UPDATE 0x1
#define MODE_BIN    0x2
#define MODE_EXCL   0x4

FILE* fopen(const char* filename, const char* mode) {
	int flags = 0;
	int i = 0;
	int modeFlags = 0;
	do {
		i++;
		switch(mode[i]) {
		case '+':
			modeFlags |= MODE_UPDATE;
			break;
		case 'b':
			modeFlags |= MODE_BIN;
			break;
		case 'x':
			modeFlags |= MODE_EXCL;
			break;
		default:
			i = 0;
			break;
		}
	} while (i != 0);

	switch(mode[0]) {
	case 'r':
		flags = O_RDWR;
		break;
	case 'w':
		flags = O_TRUNC | O_CREAT | O_WRONLY;
		if (modeFlags & MODE_EXCL) {
			flags |= O_EXCL;
		}
		break;
	case 'a':
		flags = O_CREAT | O_WRONLY;
		break;
	default:
		return NULL;
	}

	if (modeFlags & MODE_UPDATE) {
		flags &= !(O_ACCMODE);
		flags |= O_RDWR;
	}

	// TODO Check default fileMode
	int fileMode = 0x0777;

	int fd = open(filename, flags, fileMode);
	if (fd < 0) {
		errno = fd;
		return NULL;
	}
	FILE* f = malloc(sizeof(FILE));
	f->fileno = fd;
	f->flags = MAGIC_VALUE;
	f->offset = 0;
	f->eof = false;
	f->error = 0;
	f->bufVirtPos = BUF_VIRT_SIZE;
	f->fn = &file_fn;
	setvbuf(f, NULL, _IOFBF, BUFSIZ); // TODO change to full buffer or line buffer
	return f;


}

