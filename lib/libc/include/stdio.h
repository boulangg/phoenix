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

/*typedef struct FILE {
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
} FILE;*/

typedef struct FILE FILE;

extern FILE* stdout;
extern FILE* stdin;
extern FILE* stderr;

int fgetc(FILE* str);
int fputc(int character, FILE* str);
int fflush(FILE* str);

int setbuf(FILE* str, char* buffer);
int setvbuf(FILE* str, char* buffer, int mode, size_t size);

int fprintf(FILE* str, const char* format, ...);
int fscanf(FILE* str, const char* format, ...);
int printf(const char* format, ...);
int scanf(const char* format, ...);
int snprintf(char* s, size_t n, const char* format, ...);
int sprintf(char* s, const char* format, ...);
int sscanf(const char* s, const char* format, ...);
int vfprintf(FILE* str, const char* format, va_list arg);
int vfscanf(FILE* str, const char* format, va_list arg);
int vprintf(const char* format, va_list arg);
int vscanf(const char* format, va_list arg);
int vsnprintf(char* s, size_t n, const char* format, va_list arg);
int vsprintf(char* s, const char* format, va_list arg);
int vsscanf(const char* s, const char* format, va_list arg);

#ifdef __cplusplus
}
#endif

#endif // _STDIO_H_
