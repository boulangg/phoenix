#include "io.h"

#include <stdbool.h>

int _IO_file_fgetc(FILE* str) {
	// Change stream to read mode
	if (str->flags & BUFWRITE) {
		if (fflush(str)==EOF) {
			return EOF;
		}
		//fflush(str);
		str->flags &= ~BUFWRITE;
	}
	// Fill in bufferwith data from stream
	if (str->bufPos >= str->bufEnd) {
		str->offset += str->bufPos-str->bufStart;
		//lseek(str->fileno, str->offset, SEEK_SET);
		int count = read(str->fileno, str->bufStart, str->bufSize);
		if (count == 0) {
			str->eof = true;
			return EOF;
		}
		str->bufPos = str->bufStart;
		str->bufEnd = str->bufStart + count;
	}
	return *(str->bufPos++);
}

int _IO_file_fputc(int c, FILE* str) {
	// Change stream to write mode
	if (!(str->flags & BUFWRITE)) {
		str->offset += str->bufPos-str->bufStart;
		str->flags |= BUFWRITE;
		str->bufPos = str->bufStart;
		str->bufEnd = str->bufStart + str->bufSize;
	}
	// Flush if buffer full
	if (str->bufPos >= str->bufEnd) {
		if (fflush(str)==EOF) {
			return EOF;
		}
	}
	// Write new char
	*(str->bufPos++) = c;
	// Check for flush
	if (str->mode & _IONBF) {
		if (fflush(str) == EOF) {
			return EOF;
		}
	} else if ((str->mode & _IOLBF) && (c == '\n')) {
		if (fflush(str) == EOF) {
			return EOF;
		}
	}
	return c;
}

struct stream_ops file_fn = {
		_IO_file_fgetc,
		_IO_file_fputc
};
