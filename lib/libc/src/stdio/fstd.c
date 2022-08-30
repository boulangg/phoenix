#include <stdio.h>

#include <errno.h>
#include <limits.h>
#include <string.h>

FILE* stdout;
FILE* stdin;
FILE* stderr;

// Formatted input/output
int printf(const char* format, ...)
{
	va_list v;
	va_start(v, format);
	int res = vfprintf(stdout, format, v);
	va_end(v);
	return res;
}

int scanf(const char* format, ...)
{
	va_list v;
	va_start(v, format);
	int res = vfscanf(stdin, format, v);
	va_end(v);
	return res;
}
int vprintf(const char* format, va_list arg)
{
	return vfprintf(stdout, format, arg);
}
int vscanf(const char* format, va_list arg)
{
	return vfscanf(stdin, format, arg);
}

// Character input/output
int getchar()
{
	return fgetc(stdin);
}

char* gets(char* s)
{
	return fgets(s, INT_MAX, stdin);
}

int puts(const char* s)
{
	if (fputs(s, stdout) == EOF) {
		return EOF;
	}
	return fputc('\n', stdout);
}

int putchar(int c)
{
	return fputc(c, stdout);
}

void perror(const char* str)
{
	if (str != NULL) {
		fprintf(stderr, "%s: ");
	}
	fprintf(stderr, strerror(errno));
}
