/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <stdio.h>

int vprintf(const char* format, va_list arg) {
	return vfprintf(stdout, format, arg);
}
