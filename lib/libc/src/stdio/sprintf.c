#include <stdio.h>

int sprintf(char* str, const char* format, ...)
{
	va_list v;
	va_start(v, format);
	int res = vsprintf(str, format, v);
	va_end(v);
	return res;
}
