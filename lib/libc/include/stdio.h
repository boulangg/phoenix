/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

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
