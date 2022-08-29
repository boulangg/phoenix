/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <stdio.h>
#include "io.h"

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
