/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <errno.h>
#include <stddef.h>

int errno = 0;

char* sys_errlist[] = {
	"unknown"
};
int sys_nerr = 1;

char* strerror(int errnum)
{
	if (errnum > 0 && errnum < sys_nerr) {
		return sys_errlist[errnum];
	}
	return sys_errlist[0];
}
