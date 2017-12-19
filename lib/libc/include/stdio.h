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

struct File;
typedef struct FILE FILE;
typedef uint64_t fpos_t;

typedef int (*fgetc_fn)(FILE* str);
typedef int (*fputc_fn)(int c, FILE* str);
//typedef int (*fflush_fn)(FILE* str);

typedef struct stream_ops {
	fgetc_fn fgetc;
	fputc_fn fputc;
	//fflush_fn fflush;
} stream_ops;

typedef struct FILE FILE;

extern FILE* stdout;
extern FILE* stdin;
extern FILE* stderr;

// File access
int fclose(FILE* str);
int fflush(FILE* str);
FILE* fopen(const char* filename, const char* mode);
FILE* freopen(const char* filename, const char* mode, FILE* str);
int setbuf(FILE* str, char* buffer);
int setvbuf(FILE* str, char* buffer, int mode, size_t size);

// Formatted input/output
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

// Character input/output
int fgetc(FILE* str);
char* fgets(char* s, int num, FILE* str);
int fputc(int c, FILE* str);
int fputs(const char* s, FILE* str);
int getc(FILE* str);
int getchar();
char* gets(char* s);
int putc(int c, FILE* str);
int putchar(int c);
int puts(const char* s);
int ungetc(int c, FILE* str);

// Direct input/output
size_t fread(void* ptr, size_t size, size_t count, FILE* str);
size_t fwrite(const void* ptr, size_t size, size_t count, FILE* stream);

// File positionning
int fgetpos(FILE* str, fpos_t* pos);
int fseek(FILE* str, long int offset, int origin);
int fsetpos(FILE* str, const fpos_t* pos);
long int ftell(FILE* str);
void rewind(FILE* str);

// Error-handling
void clearerr(FILE* str);
int feof(FILE* str);
int ferror(FILE* str);
void perror(const char* str);

#ifdef __cplusplus
}
#endif

#endif // _STDIO_H_
