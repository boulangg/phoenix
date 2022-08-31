#include <stdio.h>

#include "io.h"

#include <stdlib.h>

int setvbuf(FILE* str, char* buffer, int mode, size_t size)
{
	CHECK_FILE(str);
	if (str->flags & (SF_READ | SF_WRITE)) {
		return -1;
	}

	if (size == 0) {
		size = 1;
	}

	str->mode = mode;
	str->bufSize = size;
	if (buffer != NULL) {
		str->bufStart = buffer;
		str->bufPos = buffer;
		str->bufEnd = buffer;
		str->flags |= SF_USERBUF;
	}

	return 0;
}

int setbuf(FILE* str, char* buffer)
{
	CHECK_FILE(str);
	if (buffer == NULL) {
		return setvbuf(str, buffer, _IONBF, BUFSIZ);
	} else {
		return setvbuf(str, buffer, _IOFBF, BUFSIZ);
	}
}
