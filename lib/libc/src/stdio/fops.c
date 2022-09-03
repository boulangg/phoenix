#include "io.h"

#include <stdlib.h>
#include <string.h>

void init_buffer(FILE* str)
{
	if (str->flags & SF_STRING) {
		return 0;
	}

	if (str->bufStart == NULL) {
		str->bufStart = malloc(str->bufSize);
		str->bufPos = str->bufStart;
		str->bufEnd = str->bufStart + str->bufSize;
	}
}

int bufWrite(FILE* str)
{
	if (str->flags & SF_STRING) {
		return EOF;
	}

	ssize_t nb = 0;
	while (nb < str->bufPos - str->bufStart) {
		ssize_t res = write(str->fileno, str->bufStart + nb, str->bufPos - str->bufStart - nb);
		if (res < 0) {
			return res;
		} else {
			nb += res;
		}
	}
	str->bufPos = str->bufStart;

	return nb;
}

int bufRead(FILE* str)
{
	int nb = read(str->fileno, str->bufStart, str->bufSize);
	if (nb >= 0) {
		str->bufPos = str->bufStart;
		str->bufEnd = str->bufStart + nb;
		str->eof = false;
	} else {
		str->bufPos = str->bufStart;
		str->bufEnd = str->bufStart;
		str->eof = true;
	}

	return nb;
}

size_t fread(void* buffer, size_t size, size_t count, FILE* str)
{
	if (size == 0 || count == 0) {
		return 0;
	}
	if (!(str->flags & (SF_READ | SF_WRITE))) {
		init_buffer(str);
	}
	if (str->flags & SF_WRITE) {
		return 0;
	}
	if (!(str->flags & SF_READ)) {
		str->flags |= SF_READ;
		str->bufPos = str->bufStart;
		str->bufEnd = str->bufStart;
	}

	char* buf = (char*)buffer;

	size_t nb = 0;
	if (str->bufUnget != EOF) {
		*buf = str->bufUnget;
		str->bufUnget = EOF;
		nb++;
	}

	while (nb < size * count) {
		size_t len = str->bufEnd - str->bufPos;
		if (len == 0) {
			if ((len = bufRead(str)) == 0) {
				return nb / size;
			}
		}

		if (len > size * count - nb) {
			len = size * count - nb;
		}
		memcpy(buf + nb, str->bufPos, len);

		nb += len;
	}

	return nb / size;
}

size_t fwrite(const void* buffer, size_t size, size_t count, FILE* str)
{
	if (size == 0 || count == 0) {
		return 0;
	}
	if (!(str->flags & (SF_READ | SF_WRITE))) {
		init_buffer(str);
	}
	if (str->flags & SF_READ) {
		return 0;
	}
	if (!(str->flags & SF_WRITE)) {
	}

	const char* buf = (const char*)buffer;

	size_t nb = 0;
	while (nb < size * count) {
		size_t len = str->bufEnd - str->bufPos;
		if (len == 0) {
			if (bufWrite(str) < 0) {
				return EOF;
			}
			len = str->bufEnd - str->bufPos;
		}

		if (len > size * count - nb) {
			len = size * count - nb;
		}

		bool flushLine = false;
		if (str->mode == _IOLBF) {
			for (size_t i = nb; i < len; i++) {
				if (buf[i] == '\n') {
					len = i + 1;
					flushLine = true;
					break;
				}
			}
		}

		memcpy(str->bufPos, buf + nb, len);
		str->bufPos += len;
		nb += len;
		if (flushLine) {
			if (bufWrite(str) < 0) {
				return EOF;
			}
		}
	}

	if (str->mode == _IONBF) {
		fflush(str);
	}

	return nb / size;
}

int fflush(FILE* str)
{
	CHECK_FILE(str);
	if (!(str->flags & SF_WRITE)) {
		str->flags &= ~(SF_READ | SF_WRITE);
		return 0;
	}

	size_t wr_size = str->bufPos - str->bufStart;
	if (wr_size > 0) {
		str->flags ^= (SF_READ | SF_WRITE);
		return bufWrite(str);
	}
	str->flags ^= (SF_READ | SF_WRITE);
	return 0;
}

int ungetc(int c, FILE* str)
{
	CHECK_FILE(str);
	if (!(str->flags & SF_READ)) {
		return EOF;
	}

	if (str->bufUnget == EOF) {
		str->bufUnget = c;
		return c;
	} else {
		return EOF;
	}
}
