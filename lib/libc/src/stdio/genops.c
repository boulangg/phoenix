/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "io.h"

#include <stdlib.h>

int fgetc(FILE* str)
{
	CHECK_FILE(str);
	// read ungetc buffer
	if (str->bufVirtPos < BUF_VIRT_SIZE) {
		int c = str->bufVirtStart[str->bufVirtPos];
		str->bufVirtPos++;
		return c;
	}
	return str->fn->fgetc(str);
}

char* fgets(char* s, int num, FILE* str)
{
	int i = 0;
	while (i < num - 1) {
		int c = fgetc(str);
		if (c == EOF) {
			if (feof(str) && i == 0) {
				return NULL;
			}
			if (ferror(str)) {
				return NULL;
			}
		}
		s[i] = (char)c;
		i++;
		if (c == '\n') {
			break;
		}
	}
	s[i] = '\0';
	return s;
}

int fputc(int c, FILE* str)
{
	CHECK_FILE(str);
	return str->fn->fputc(c, str);
}

int fputs(const char* s, FILE* str)
{
	CHECK_FILE(str);
	int i = 0;
	while (s[i] != '\0') {
		if (fputc(s[i], str) == EOF) {
			return EOF;
		}
		i++;
	}
	return 0;
}

int getc(FILE* str)
{
	return fgetc(str);
}

int getchar()
{
	return fgetc(stdin);
}

char* gets(char* s)
{
	int i = 0;
	int c = fgetc(stdin);
	while (c != '\n') {
		if (c == EOF) {
			if (feof(stdin) && i == 0) {
				return NULL;
			}
			if (ferror(stdin)) {
				return NULL;
			}
		}
		s[i] = (char)c;
		i++;
	}
	s[i] = '\0';
	return s;
}

int putc(int c, FILE* str)
{
	return fputc(c, str);
}

int putchar(int c)
{
	return fputc(c, stdout);
}

int puts(const char* s)
{
	if (fputs(s, stdout) == EOF) {
		return EOF;
	}
	return fputc('\n', stdout);
}

/*static int _IO_putback_fail(int c, FILE* str) {
	// TODO add a buffer for ungetc
	(void)c;
	(void)str;
	return EOF;
}*/

int ungetc(int c, FILE* str)
{
	CHECK_FILE(str);
	if (str->flags & BUFWRITE) {
		return EOF;
	}
	if (str->bufVirtPos > 0) {
		str->bufVirtPos--;
		str->bufVirtStart[str->bufVirtPos] = (char)c;
		return c;
	}
	return EOF;
	/*if (str->bufPos > str->bufStart) {
		str->bufPos--;
		*(str->bufPos) = (char)c;
		return c;
	} else {
		return _IO_putback_fail(c, str);
	}*/
}

int fflush(FILE* str)
{
	CHECK_FILE(str);
	if (str->flags & BUFWRITE) {
		size_t wr_size = str->bufPos - str->bufStart;
		if (wr_size > 0) {
			//lseek(str->fileno, 0, SEEK_END);
			if ((size_t)write(str->fileno, str->bufStart, wr_size) != wr_size) {
				return EOF;
			}
			str->bufPos -= wr_size;
		}
		return wr_size;
	}
	return 0;
}

int fclose(FILE* str)
{
	CHECK_FILE(str);
	fflush(str);
	if (!(str->flags & USERBUF)) {
		free(str->bufStart);
	}
	return close(str->fileno);
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

int setvbuf(FILE* str, char* buffer, int mode, size_t size)
{
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

int setbuf(FILE* str, char* buffer)
{
	CHECK_FILE(str);
	if (buffer == NULL) {
		return setvbuf(str, buffer, _IONBF, BUFSIZ);
	} else {
		return setvbuf(str, buffer, _IOFBF, BUFSIZ);
	}
}
