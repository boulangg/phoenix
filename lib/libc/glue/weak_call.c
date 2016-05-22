/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "weak_call.h"

void *sbrk(ptrdiff_t increment) {
	(void)increment;
	return 0;
}

void exit(int status) {
	(void)status;
	__builtin_unreachable();
}
