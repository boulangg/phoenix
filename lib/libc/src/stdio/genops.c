
#include "io.h"

int fgetc(FILE* str) {
	CHECK_FILE(str);
	return str->fn->fgetc(str);
}

int fputc(int c, FILE* str) {
	CHECK_FILE(str);
	return str->fn->fputc(c, str);
}

int fflush(FILE* str) {
	CHECK_FILE(str);
	if (str->flags & BUFWRITE) {
		size_t wr_size = str->bufPos-str->bufStart;
		if (wr_size > 0) {

			if (write(str->fileno, str->bufStart, wr_size) != wr_size) {
				return EOF;
			}
			str->bufPos -= wr_size;
		}
		return wr_size;
	}
	return 0;
}

int fclose(FILE* str) {
	CHECK_FILE(str);
	fflush(str);
	if (!(str->flags & USERBUF)) {
		free(str->bufStart);
	}
	return close(str->fileno);
}

int feof(FILE* str) {
	CHECK_FILE(str);
	return str->eof;
}

int ferror(FILE* str) {
	CHECK_FILE(str);
	return str->error;
}

int setvbuf(FILE* str, char* buffer, int mode, size_t size) {
	CHECK_FILE(str);
	if (size == 0) {
		size = 1;
	}
	str->mode = mode;
	if (!(str->flags & USERBUF) && str->bufStart != NULL) {
		free(str->bufStart);
	}
	if (buffer != NULL) {
		str->flags |= USERBUF;
		str->bufStart = buffer;
	} else {
		str->flags &= ~USERBUF;
		str->bufStart = malloc(size);
		if (str->bufStart == NULL) {
			return EOF;
		}
	}
	str->bufPos = str->bufStart;
	str->bufEnd = str->bufStart;
	str->bufSize = size;
	return 0;
}

int setbuf(FILE * str, char* buffer) {
	CHECK_FILE(str);
	if (buffer == NULL) {
		return setvbuf(str, buffer, _IONBF, BUFSIZ);
	} else {
		return setvbuf(str, buffer, _IOFBF, BUFSIZ);
	}
}
