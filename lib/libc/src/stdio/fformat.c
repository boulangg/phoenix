#include <stdio.h>

int fprintf(FILE* str, const char* format, ...)
{
	va_list v;
	va_start(v, format);
	int res = vfprintf(str, format, v);
	va_end(v);
	return res;
}

int fscanf(FILE* str, const char* format, ...)
{
	va_list v;
	va_start(v, format);
	int res = vfscanf(str, format, v);
	va_end(v);
	return res;
}
