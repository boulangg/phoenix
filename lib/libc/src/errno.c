/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <errno.h>
#include <stddef.h>

int errno=0;

char* strerror(int errnum) {
	// TODO not yet implemented
	(void)errnum;
	return NULL;
}
