/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef _STDIO_H_
#define _STDIO_H_

#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

#define EOF (-1)
#define _IOFBF 0x1
#define _IOLBF 0x2
#define _IONBF 0x4

#define BUFSIZ 4096

typedef struct FILE {
	int64_t flags;
	char* readBufStart;
	char* readBufPos;
	char* readBufEnd;
	int readBufSize;
	char* writeBufStart;
	char* writeBufPos;
	char* writeBufEnd;
	int writeBufSize;
	int64_t fileno;
	int64_t mode;
	int64_t offset;
	bool eof;
	int error;
} FILE;

extern FILE* stdout;
extern FILE* stdin;
extern FILE* stderr;

int fputc(int character, FILE* stream);
int fflush(FILE* stream);

int sprintf(char * s, const char * format, ...);

int vsprintf(char *s, const char *format, va_list arg);

#ifdef __cplusplus
}
#endif

#endif // _STDIO_H_
