#include <stdio.h>

#include "io.h"

#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

#define MODE_UPDATE 0x1
#define MODE_BIN    0x2
#define MODE_EXCL   0x4

FILE* fopen(const char* filename, const char* mode)
{
	int flags = 0;
	int i = 0;
	int modeFlags = 0;
	do {
		i++;
		switch (mode[i]) {
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

	switch (mode[0]) {
	case 'r':
		if (modeFlags & MODE_UPDATE) {
			flags = O_RDWR;
		} else {
			flags = O_RDONLY;
		}
		break;
	case 'w':
		flags = O_TRUNC | O_CREAT;
		if (modeFlags & MODE_EXCL) {
			flags |= O_EXCL;
		}
		if (modeFlags & MODE_UPDATE) {
			flags = O_RDWR;
		} else {
			flags = O_WRONLY;
		}
		break;
	case 'a':
		flags = O_APPEND | O_CREAT;
		if (modeFlags & MODE_UPDATE) {
			flags = O_RDWR;
		} else {
			flags = O_WRONLY;
		}
		break;
	default:
		return NULL;
	}

	int fileMode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

	int fd = open(filename, flags, fileMode);
	if (fd < 0) {
		errno = fd;
		return NULL;
	}
	FILE* f = malloc(sizeof(FILE));
	f->fileno = fd;
	init_file(f);
	setvbuf(f, NULL, _IOFBF, BUFSIZ);
	return f;
}

void clearerr(FILE* str)
{
	str->error = false;
}

int feof(FILE* str)
{
	CHECK_FILE(str);
	return str->eof;
}

int ferror(FILE* str)
{
	CHECK_FILE(str);
	return str->error;
}

int fclose(FILE* str)
{
	CHECK_FILE(str);
	fflush(str);
	if (!(str->flags & SF_USERBUF) && str->bufStart == NULL) {
		free(str->bufStart);
	}
	return close(str->fileno);
}
