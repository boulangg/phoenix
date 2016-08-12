/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <stdio.h>
#include "io.h"

int vsnprintf(char* s, size_t n, const char* format, va_list arg) {
	if (n == 0) {
		return -1;
	}
	FILE str;
	bufToFile(&str, s, n-1);
	int res = vfprintf(&str, format, arg);
	return res;
}
