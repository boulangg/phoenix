/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "io.h"

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
