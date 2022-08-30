#include <stdio.h>

#include "io.h"

void bufToFile(FILE* str, char* s, size_t n)
{
	(void)str;
	(void)s;
	(void)n;
	/*str->flags = MAGIC_VALUE;
	str->bufStart = s;
	str->bufPos = s;
	str->bufEnd = s;
	str->bufSize = n;
	// Check overflow
	if (str->bufStart + str->bufSize < str->bufStart) {
		str->bufSize = (size_t)((char*)-1 - str->bufStart);
	}
	str->bufVirtPos = BUF_VIRT_SIZE;
	str->eof = false;
	str->fn = &str_fn;*/
}

int vsscanf(const char* s, const char* format, va_list arg)
{
	FILE str;
	bufToFile(&str, (char*)s, (size_t)-1);
	int res = vfscanf(&str, format, arg);
	return res;
}

int vsprintf(char* s, const char* format, va_list arg)
{
	FILE str;
	bufToFile(&str, s, (size_t)-1);
	int res = vfprintf(&str, format, arg);
	if (res > 0) {
		s[res] = '\0';
	}
	return res;
}

int vsnprintf(char* s, size_t n, const char* format, va_list arg)
{
	if (n == 0) {
		return -1;
	}
	FILE str;
	bufToFile(&str, s, n - 1);
	int res = vfprintf(&str, format, arg);
	if (res > 0) {
		s[res] = '\0';
	}
	return res;
}

int sscanf(const char* s, const char* format, ...)
{
	va_list v;
	va_start(v, format);
	FILE str;
	bufToFile(&str, (char*)s, (size_t)-1);
	int res = vfscanf(&str, format, v);
	va_end(v);
	return res;
}

int sprintf(char* s, const char* format, ...)
{
	va_list v;
	va_start(v, format);
	FILE str;
	bufToFile(&str, s, (size_t)-1);
	int res = vfprintf(&str, format, v);
	if (res >= 0) {
		s[res] = '\0';
	}
	va_end(v);
	return res;
}

int snprintf(char* s, size_t n, const char* format, ...)
{
	if (n == 0) {
		return -1;
	}
	va_list v;
	va_start(v, format);
	FILE str;
	bufToFile(&str, s, n - 1);
	int res = vfprintf(&str, format, v);
	if (res >= 0) {
		if ((size_t)res < n - 1) {
			s[res] = '\0';
		} else {
			s[n - 1] = '\0';
		}
	}
	va_end(v);
	return res;
}
