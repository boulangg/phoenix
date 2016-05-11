#ifndef _STDIO_H_
#define _STDIO_H_

#include <stdarg.h>

int sprintf(char * s, const char * format, ...);

int vsprintf(char *s, const char *format, va_list arg);

#endif // _STDIO_H_
