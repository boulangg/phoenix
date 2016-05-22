#ifndef _STDIO_H_
#define _STDIO_H_

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

int sprintf(char * s, const char * format, ...);

int vsprintf(char *s, const char *format, va_list arg);

#ifdef __cplusplus
}
#endif

#endif // _STDIO_H_
