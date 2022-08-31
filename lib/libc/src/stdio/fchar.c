#include <stdio.h>

#include "io.h"

#include <errno.h>

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
			return s;
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
	return i;
}

int fgetc(FILE* str)
{
	CHECK_FILE(str);
	char c;
	size_t nbChar = fread(&c, 1, 1, str);
	if (nbChar == 0) {
		return EOF;
	}
	return c;
}

int getc(FILE* str) __attribute__((alias("fgetc")));

int fputc(int c, FILE* str)
{
	CHECK_FILE(str);
	size_t nbChar = fwrite(&c, 1, 1, str);
	if (nbChar == 0) {
		return EOF;
	}
	return c;
}

int putc(int c, FILE* str) __attribute__((alias("fputc")));
