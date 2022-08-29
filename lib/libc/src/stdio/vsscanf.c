/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <stdio.h>
#include "io.h"

int vsscanf(const char* s, const char* format, va_list arg)
{
	FILE str;
	bufToFile(&str, (char*)s, (size_t)-1);
	int res = vfscanf(&str, format, arg);
	return res;
}
